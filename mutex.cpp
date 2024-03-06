#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

using namespace std;

const int arrSize = 9;
vector<int> sharedArr = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
mutex m;
condition_variable cv;
int curThreadIdx = 1;

void calcSum(int idx) {
    unique_lock<mutex> lock(m);

    while (curThreadIdx != idx) {
        cv.wait(lock);
    }

    int sum = 0;
    for (int i : sharedArr) {
        sum += i;
    }

    this_thread::sleep_for(chrono::milliseconds(500));

    cout << "Thread " << idx << " completed action." << endl;

    cout << "sum: " << sum << "\n" << endl;

    curThreadIdx = (curThreadIdx % thread::hardware_concurrency()) + 1;

    cv.notify_all();
}

int main() {
    vector<thread> threads;

    for (int i = 1; i <= thread::hardware_concurrency(); ++i) {
        threads.emplace_back(calcSum, i);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
