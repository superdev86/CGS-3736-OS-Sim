// Christopher Stephenson
// P8 OS Management
// 4/13/24

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#define EXIT 0
#define INVALID -1
#define CPU 1
#define PID 5
#define RACE 2
#define BANKER 3
#define PROCESS 5
#define RESOURCE 3
#define TRUE 1
#define FALSE 0
#define MEMORY 4
#define FIRST 0
#define BEST 1
#define WORST 2
#define NEXT 3
#define PAGE 5
#define FIFO 0
#define LRU 0
#define FRAMES 4
#define DISK 6
#define FCFS 0
#define SSTF 1
#define REQUEST 8

// function prototypes
int displayMenu();
void cpuScheduling();
void fcfs(int process[], int at[], int bt[]);
void sjf(int process[], int at[], int bt[]);
void displaySchedule(int process[], int at[], int bt[], int wt[], int tat[]);
void raceCondition();
void* threadFuncOne();
void* threadFuncTwo();
void bankersAlgorithm();
void systemState(int feasible[], int safe[]);
void memoryManagement();
void displayProcess();
void firstFit(int blockSize[], int blocks, int processSize[], int processes);
void worstFit(int blockSize[], int blocks, int processSize[], int processes);
void bestFit(int blockSize[], int blocks, int processSize[], int processes);
void nextFit(int blockSize[], int blocks, int processSize[], int processes);
void pageReplacement();
void lru();
int findLRU(int time[]);
void fifo();
void displayPages(int page, int allocation[]);

void diskScheduling();
void diskFefs();
void diskSstf();


int resource = 5;

int main() {
    int choice = INVALID;

    while(choice != EXIT) {
        // call the displayMenu function to get user's choice
        choice = displayMenu();
        switch(choice) {
            case CPU:
                // call the cpuScheduling function
                cpuScheduling();
                break;
            case RACE:
                // call the raceCondition function
                raceCondition();
                break;
            case BANKER:
                //call bankerAlgorithm function
                bankersAlgorithm();
                break;
            case MEMORY:
                // call memoryManagement function
                memoryManagement();
                break;
            case PAGE:
                // call pageReplacement function
                pageReplacement();
            case DISK:
                //call diskScheduling function
                diskScheduling();
            case EXIT:
                // exit the program
                exit(EXIT);
        }
    }
    return 0;
}

// function to display the menu and get user's choice
int displayMenu() {
    int choice = INVALID;

    while (choice == INVALID) {
        // display the menu options
        printf("\t\t\t\t********** Operating System Management Menu **********\n");
        printf("Select the OS program to run, enter the number of your selection.\n");
        printf("1. CPU Scheduling\n");
        printf("2. Race Condition\n");
        printf("3. Banker's Algorithm\n");
        printf("4. Memory Management\n");
        printf("5. Page Replacement\n");
        printf("6. Disk Scheduling\n");
        printf("0. Exit\n");

        // get user's choice
        scanf("%d", &choice);

        if (choice == EXIT) {
            // if the user chooses to exit, return EXIT
            return EXIT;
        }
        else if (choice == CPU) {
            return choice;
        }
        else if(choice == RACE) {
            return choice;
        }
        else if (choice == BANKER) {
            return choice;
        }
        else if (choice == MEMORY) {
            return choice;
        }
        else if (choice == PAGE) {
            return choice;
        }
        else if (choice == DISK) {
            return choice;
        }
        else {
            // if the choice is neither CPU Scheduling,Race, or Exit, return INVALID
            return INVALID;
        }
    }
}

// function to perform CPU Scheduling
void cpuScheduling() {
    int process[] = {1, 2, 3, 4, 5};
    int arrvTime[] = {0, 2, 4, 6, 7};
    int burstTime[] = {8, 5, 10, 2, 3};

    // call the fcfs and sjf funtions
    fcfs(process, arrvTime, burstTime);
    sjf(process, arrvTime, burstTime);
}

// function to implement first come first served algo
void fcfs(int process[], int at[], int bt[]) {
    // arrays to store waiting time and turnaround time
    int wt[PID] = {0};
    int tat[PID];

    int i;
    // calculate waiting time
    for (i = 1; i < PID; i++) {
        wt[i] = bt[i - 1] + wt[i - 1];
    }
    // calculate turnaround time
    for (i = 0; i < PID; i++) {
        tat[i] = bt[i] + wt[i];
    }

    // display the scheduling table
    displaySchedule(process, at, bt, wt, tat);
}

// function to implement shortest job first algo
void sjf(int process[], int at[], int bt[]) {
    // arrays to store waiting time and turnaround time
    int wt[PID];
    int tat[PID];
    int idx;
    int temp;

    int i, j;

    for (i = 0; i < PID; i++) {
        idx = i;
        for (j = i + 1; j < PID; j++) {
            if (bt[j] < bt[idx]) {
                idx = j;
            }
        }

        // swap burst time
        temp = bt[i];
        bt[i] = bt[idx];
        bt[idx] = temp;

        // swap process ids
        temp = process[i];
        process[i] = process[idx];
        process[idx] = temp;

        // swap arrival times
        temp = at[i];
        at[i] = at[idx];
        at[idx] = temp;
    }

    wt[0] = 0;

    // calculate waiting time
    for (i = 1; i < PID; i++) {
        wt[i] = bt[i - 1] + wt[i - 1];
    }
    // calculate turnaround time
    for (i = 0; i < PID; i++) {
        tat[i] = bt[i] + wt[i];
    }

    // display the scheduling table
    displaySchedule(process, at, bt, wt, tat);
}

// function to display the scheduling table and average times
void displaySchedule(int process[], int at[], int bt[], int wt[], int tat[]) {
    int totWt = 0;
    int totTat = 0;
    float avgWt = 0;
    float avgTat = 0;

    int i;
    printf("\n");
    for (i=0; i < PID; i++) {
        totWt += wt[i];
        totTat += tat[i];

        // display process details
        printf("%d %d %d %d %d\n",process[i], at[i], bt[i], wt[i], tat[i]);
    }
    // calculate average waiting time and average turnaround time
    avgWt = (float)totWt / PID;
    avgTat = (float)totTat / PID;

    // display average times
    printf("Average Waiting Time: %.2f", avgWt);
    printf("\nAverage Turnaround Time: %.2f\n", avgTat);
}

void raceCondition() {

    pthread_t threadOne;
    pthread_t threadTwo;

    pthread_create(&threadOne, NULL, threadFuncOne, NULL);
    pthread_create(&threadTwo, NULL, threadFuncTwo, NULL);

    pthread_join(threadOne, NULL);
    pthread_join(threadTwo, NULL);

    printf("Value of shared resource: %d\n", resource);
}

void* threadFuncOne() {

    int threadId = 1;
    int fOne = resource;

    printf("Thread 1 reads the value of shared resource as: %d\n", fOne);
    fOne++;
    printf("Local update by Thread 1: %d\n", fOne);

    sleep(1);
    resource = fOne;

    printf("Value of shared resource updated by Thread 1: %d\n", resource);
}

void* threadFuncTwo() {
    int threadId = 2;
    int fTwo = resource;

    printf("Thread 2 reads resource as: %d\n", resource);
    fTwo--;
    printf("Local update by Thread 2: %d\n", fTwo);

    sleep(1);
    resource = fTwo;

    printf("Value of shared resource updated by Thread 2: %d\n", resource);
}

void bankersAlgorithm() {

    int allocation[PROCESS][RESOURCE] = {
        { 0, 0, 2 }, // p0
        { 3, 0, 2 }, // p1
        { 0, 1, 0 }, // p2
        { 2, 1, 1 }, // p3
        { 2, 0, 0 } // p4
    };

    int maxDemand[PROCESS][RESOURCE] = {
        { 4, 3, 3 }, // p0
        { 9, 0, 2 }, // p1
        { 7, 5, 3 }, // p2
        { 2, 2, 2 }, // p3
        { 3, 2, 2 } // p4
    };

    int available[RESOURCE] = {2, 4, 6};
    int need[PROCESS][RESOURCE];
    int feasible[PROCESS];
    int safe[PROCESS];
    int safeIdx = 0;


    // initialize feasible array
    for (int p = 0; p < PROCESS; p++) {
        feasible[p] = FALSE;
    }

    // initialize need array
    for (int p = 0; p < PROCESS; p++) {
        for (int r = 0; r < RESOURCE; r++) {
            need[p][r] = maxDemand[p][r] - allocation[p][r];
        }
    }


    // banker’s algorithm
    for (int i = 0; i < PROCESS; i++) {
        for (int p = 0; p < PROCESS; p++) {
            if (feasible[p] == FALSE) {
                int isUnsafe = FALSE;

                // check if the process can be satisfied
                for (int r = 0; r < RESOURCE; r++) {
                    if (need[p][r] > available[r]) {
                        isUnsafe = TRUE;
                        break;
                    }
                }

                // if process can be satisfied, mark as feasible
                if (isUnsafe == FALSE) {
                    safe[safeIdx++] = p;

                    // release resources for other processes
                    for (int r = 0; r < RESOURCE; r++) {
                        available[r] += allocation[p][r];
                    }

                    feasible[p] = TRUE;
                }
            }
        }
    }

    // call function to check the system state
    systemState(feasible, safe);
}

void systemState(int feasible[], int safe[]) {

    int isSafe = TRUE;
    int i;

    // loop through the processes
    for (i = 0; i < PROCESS; i++) {
        if (feasible[i] == FALSE) {
            isSafe = FALSE;
            printf("The operating system is not safe.");
            break;
        }
    }

    // if the system is safe
    if (isSafe == TRUE) {
        for (i = 0; i < PROCESS; i++) {
            // print the process number
            printf("P%d ", safe[i]);
            if ( i < PROCESS - 1) {
            printf("-> ");
            }
        }
        printf("\n");
    }
}

void memoryManagement() {

    int blockSize[] = {70, 20, 45, 65, 40, 80};
    int processSize[] = {15, 35, 25, 45, 60, 20};
    int blocks = sizeof(blockSize) / sizeof(blockSize[0]);
    int processes = sizeof(processSize) / sizeof(processSize[0]);

    printf("\t******** Memory Management ********\n\n");
    for (int algorithm = FIRST; algorithm <= NEXT; algorithm++) {
        int blockSize[] = {70, 20, 45, 65, 40, 80};
        int processSize[] = {15, 35, 25, 45, 60, 20};
        switch (algorithm) {
            case FIRST:
                // call firstFit function
                printf("\n\t******** First Fit ********\n\n");
                firstFit(blockSize, blocks, processSize, processes);
                break;
            case BEST:
                // call bestFit function
                printf("\n\t******** Best Fit ********\n\n");
                bestFit(blockSize, blocks, processSize, processes);
                break;
            case WORST:
                // call worstFit function
                printf("\n\t******** Worst Fit ********\n\n");
                worstFit(blockSize, blocks, processSize, processes);
                break;
            case NEXT:
                // call nextFit function
                printf("\n\t******** Next Fit ********\n\n");
                nextFit(blockSize, blocks, processSize, processes);
                break;
        }
    }
}

void nextFit(int blockSize[], int blocks, int processSize[], int processes) {

    int allocation[processes];
    int id = 0;
    int i;

    // call memset
    memset(allocation, INVALID, sizeof(allocation));

    for (i = 0; i < processes; i++) {
        while (id < blocks) {
            if (blockSize[id] >= processSize[i]) {
                allocation[i] = id;
                blockSize[id] -= processSize[i];
                break;
            }
            id = (id + 1) % blocks;
        }
    }

    // call displayProcess
    displayProcess(allocation, processes, processSize);
}

void firstFit(int blockSize[], int blocks, int processSize[], int processes) {

    int allocation[processes];
    memset(allocation, INVALID, sizeof(allocation));
    int i, j;

    for (i = 0; i < processes; i++) {
        for (j = 0; j < blocks; j++) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                break;
            }
        }
    }

    // call displayProcess
    displayProcess(allocation, processes, processSize);
}

void bestFit(int blockSize[], int blocks, int processSize[], int processes) {

    int allocation[processes];
    memset(allocation, INVALID, sizeof(allocation));

    for (int i = 0; i < processes; i++) {
        int bestIdx = INVALID;

        for (int j = 0; j < blocks; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (bestIdx == INVALID || blockSize[j] < blockSize[bestIdx]) {
                    bestIdx = j;
                }
            }
        }

        if (bestIdx != INVALID) {
            allocation[i] = bestIdx;
            blockSize[bestIdx] -= processSize[i];
        }
    }

    // call displayProcess
    displayProcess(allocation, processes, processSize);
}

void worstFit(int blockSize[], int blocks, int processSize[], int processes) {

    int allocation[processes];
    memset(allocation, INVALID, sizeof(allocation));

    for (int i = 0; i < processes; i++) {
        int wstIdx = INVALID;

        for (int j = 0; j < blocks; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (wstIdx == INVALID || blockSize[j] > blockSize[wstIdx]) {
                    wstIdx = j;
                }
            }
        }

        if (wstIdx != INVALID) {
            allocation[i] = wstIdx;
            blockSize[wstIdx] -= processSize[i];
        }
    }

    // call displayProcess
    displayProcess(allocation, processes, processSize);
}

void displayProcess(int allocation[], int processes, int processSize[]) {
    printf("Process No.\tProcess Size\tBlock No.\n");
    for (int i = 0; i < processes; i++) {
        printf("%d\t\t%d\t\t", i + 1, processSize[i]);
        if (processSize[i] == INVALID) {
            printf("Not Allocated\n");
        } else {
            printf("%d\n", allocation[i] + 1);
        }
    }
}

void pageReplacement() {
    printf("\n******** Page Replacement ********\n");
    for (int algorithm = 1; algorithm <= 2; algorithm++) {
        switch (algorithm) {
            case 1:
                fifo();
                break;
            case 2:
                lru();
                break;
        }
    }
}

void fifo() {
    printf("\n******** First in First Out ********\n\n");

    int pageRequests[] = {2, 3, 8, 4, 5, 6, 5, 7, 1, 8, 3, 1, 4, 2, 6};
    int pageFaults = 0;
    int allocation[FRAMES];
    int present;
    int pages = sizeof(pageRequests) / sizeof(pageRequests[0]);

    memset(allocation, INVALID, sizeof(allocation));

    int j;
    for (int i = 0; i < pages; i++) {
        present = 0;
        for (j = 0; j < FRAMES; j++) {
            if (pageRequests[i] == allocation[j]) {
                present = 1;
                pageFaults--;
                break;
            }
        }

        pageFaults++;

        if (pageFaults <= FRAMES && present == 0) {
            allocation[pageFaults - 1] = pageRequests[i];
        } else if (present == 0) {
            allocation[(pageFaults - 1) % FRAMES] = pageRequests[i];
        }

        displayPages(pageRequests[i], allocation);
    }

    printf("Total page faults: %d\n", pageFaults);
}

void lru() {
    printf("\n******** Least Recently Used ********\n\n");

    int pageRequests[] = {2, 3, 8, 4, 5, 6, 5, 7, 1, 8, 3, 1, 4, 2, 6};
    int pageFaults = 0;
    int allocation[FRAMES];
    int pages = sizeof(pageRequests) / sizeof(pageRequests[0]);
    int counter = 0;
    int time[FRAMES];

    memset(allocation, INVALID, sizeof(allocation));
    memset(time, 0, sizeof(time));

    for (int i = 0; i < pages; i++) {
        int flag1 = 0, flag2 = 0;

        for (int j = 0; j < FRAMES; j++) {
            if (pageRequests[i] == allocation[j]) {
                counter++;
                time[j] = counter;
                flag1 = 1;
                flag2 = 1;
                break;
            }
        }

        if (flag1 == 0) {
            for (int j = 0; j < FRAMES; j++) {
                if (allocation[j] == INVALID) {
                    counter++;
                    pageFaults++;
                    allocation[j] = pageRequests[i];
                    time[j] = counter;
                    flag2 = 1;
                    break;
                }
            }
        }

        if (flag2 == 0) {
            int position = findLRU(time);
            counter++;
            pageFaults++;
            allocation[position] = pageRequests[i];
            time[position] = counter;
        }

        displayPages(pageRequests[i], allocation);
    }

    printf("Total page faults: %d\n", pageFaults);
}

int findLRU(int time[]) {
    int position = 0;
    int minimum = time[0];

    for (int i = 1; i < FRAMES; ++i) {
        if (time[i] < minimum) {
            minimum = time[i];
            position = i;
        }
    }

    return position;
}

void displayPages(int page, int allocation[]) {
    printf("Page\t");
    for (int i = 0; i < FRAMES; ++i) {
        printf("Frame %d", i + 1);
        if (i < FRAMES - 1)
            printf("\t\t");
    }
    printf("\n");

    printf("%d\t", page);
    for (int i = 0; i < FRAMES; ++i) {
        if (allocation[i] == INVALID)
            printf("%s\t\t", "-");
        else
            printf("%d\t\t", allocation[i]);
    }
    printf("\n\n");
}

void diskScheduling() {
    int requests[] = {146, 89, 24, 70, 102, 13, 51, 134};
    int head = 50;
    int algorithm;
    printf("******** Disk Scheduling ********\n");

    for (algorithm = 0; algorithm < 2; ++algorithm) {
        if (algorithm == 0) {
            diskFcfs(requests, head);
        } else if (algorithm == 1) {
            diskSstf(requests, head);
        }
    }
}

void diskFcfs(int requests[], int head) {
    int seek = 0;
    int track = 0;
    int distance = 0;
    int start = head;

    printf("\n******** FCFS ********\n");
    printf("Seek Sequence: %d -> ", start);

    for (int i = 0; i < REQUEST; ++i) {
        track = requests[i];

        distance = abs(head - track);
        seek += distance;
        head = track;

        printf("%d ", head);

        if ( i < REQUEST - 1) {
            printf("-> ");
            }
    }

    printf("\nTotal seek operations: %d\n", seek);
}

void diskSstf(int requests[], int head) {
    int sequence[REQUEST] = {0};
    int distance[REQUEST] = {0};
    int seek = 0;
    int minVal = 0;
    int minValIdx = 0;
    int seqIdx = 0;

    printf("\n******** SSTF ********\n");
    printf("Seek Sequence: %d -> ", head);

    for (int i = 0; i < REQUEST; ++i) {
        for (int j = 0; j < REQUEST; ++j) {
            distance[j] = abs(head - requests[j]);
        }

        minVal = distance[0];
        minValIdx = 0;
        for (int j = 1; j < REQUEST; ++j) {
            if (minVal > distance[j]) {
                minVal = distance[j];
                minValIdx = j;
            }
        }

        sequence[seqIdx] = requests[minValIdx];
        seqIdx++;

        head = requests[minValIdx];
        requests[minValIdx] = 999;
        seek += minVal;


        printf("%d ", sequence[seqIdx - 1]);

        if ( i < REQUEST - 1) {
            printf("-> ");
            }
    }

    printf("\nTotal seek operations: %d\n", seek);
}

