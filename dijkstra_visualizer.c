/* Dijkstra's Algorithm Visualization in C
   By Kenneth Sunjaya
   6 January 2024 
   
   Controls :
   <enter>   : randomize maze
   <q>       : quit
   <space>   : view all possible dest's solutions
   <any key> : repeat visualization
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <windows.h>
#include <conio.h>
#include <unistd.h>
#include <time.h>

#define gotoxy(x,y) printf("\033[%d;%dH", y, x)
#define SIDE 32  // maze size
#define DELAY 15 // time (in milliseconds) between next path being visualized
#define CHANCE 2 // decrease this to add more walls to the map

const int text_x = SIDE*2 + 12;
const int text_y = SIDE/2 + 2;
const int VERTEX = SIDE*SIDE;

int dist[SIDE*SIDE];
int maze[SIDE + 2][SIDE + 2] = {0};
char graph[SIDE*SIDE][SIDE*SIDE] = {0};

int source;

typedef struct Node{
    int data;
    struct Node *next;
} Node;

// create an array of linked list to store shortest paths
Node *head[SIDE*SIDE] = {NULL};

Node *createNewNode(int x) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->data = x;
    newNode->next = NULL;
    return newNode;
}

Node *insert_node(Node *curr, int x) {
    if (curr == NULL) {
        return createNewNode(x);
    }
    curr->next = insert_node(curr->next, x);
    return curr;
}

// delete a linked list
void pop(int index) { 
    if (head[index] == NULL) {
        return;
    }
    Node *nodeToDelete = head[index];
    head[index] = head[index]->next;
    free(nodeToDelete);
    nodeToDelete = NULL;
    pop(index);
}

int randint(int a, int b) { // inclusive
    return ((rand() % (b - a + 1) + a));
}

void printOut(Node *curr) {
    while (curr != NULL) {
        printf("%d", curr->data);
        if (curr->next != NULL) {
            printf(" - ");
        }
        curr = curr->next;
    }
}

void printSolution(int dist[]) {
    system("cls");
    printf("Destination \t Distance \t Path\n");
    for (int i = 0; i < VERTEX; i++) {
        if (dist[i] != INT_MAX) {
            printf("%d \t\t %d \t\t ", i, dist[i]);
            printOut(head[i]);
        }
        else {
            printf("%d \t\t %s \t\t ", i, "IMPOSSIBLE");  
            printf("NULL");
        }
        puts("");
    }
}
 

int minDistance(int dist[], bool sptSet[]) {
    int min = INT_MAX, min_index, v;
    for (v=0;v<VERTEX;v++) {
        if (!sptSet[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(char graph[VERTEX][VERTEX], int src) {
    int i, count, v;
    bool sptSet[VERTEX]; 
    for (i=0;i<VERTEX;i++) {
        dist[i] = INT_MAX;
        sptSet[i] = false;
        head[i] = insert_node(head[i], src);
    }

    dist[src] = 0;

    for (count=0;count<VERTEX-1;count++) {
        int u = minDistance(dist, sptSet);
        sptSet[u] = true;
        for (v=0;v<VERTEX;v++) {
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                pop(v);
                Node *curr = head[u];
                while (curr) { // replace head[v] dengan head[u]
                    head[v] = insert_node(head[v], curr->data);
                    curr = curr->next;
                }
                head[v] = insert_node(head[v], v);
                dist[v] = dist[u] + graph[u][v];
            }
        }    
    }
}


void printMaze() {
    system("cls");
    int i, j;
    printf("   \033[1m\033[36m01");
    for (i=2;i<=SIDE;i++) {
        if (i%2 == 1) {
            printf("\033[1m\033[36m");
        }
        else {
            printf("\033[1m\033[33m");
        }
        printf("%02d", i);
    }
    printf("\033[0m");
    puts("");
    for (i=0;i<=SIDE+1;i++) {
        printf(" ");
        for (j=0;j<=SIDE+1;j++) {
            if (maze[i][j] == 1) {
                printf("  ");
            }
            else if (maze[i][j] == 2) {
                printf("\033[1m\033[33m[]\033[0;37m");
            }
            else if (maze[i][j] == 3) {
                printf("\033[0;33mX0\033[0;37m");
            }
            else if (maze[i][j] == 4) {
                printf("\033[1m\033[33mX0\033[0;37m");
            }
            else {
                printf("%c%c", 219, 219);
            }
        }
        if (i != 0 && i != SIDE+1) {
            if (i%2 == 1) {
                printf("\033[1m\033[36m");
            }
            else {
                printf("\033[1m\033[33m");
            }
            printf("%02d", i);
            printf("\033[0m");
        }
        puts("");
    }
}

void emptyMaze() {
    int i, j;
    for (i=0;i<SIDE+2;i++) {
        for (j=0;j<SIDE+2;j++) {
            maze[i][j] = 0;
        }
    }
    for (i=0;i<SIDE*SIDE;i++) {
        for (j=0;j<SIDE*SIDE;j++) {
            graph[i][j] = 0;
        }
    }
}

void randomMaze() {
    int i, j, baris = 0;
    emptyMaze();
    
    for (i=1;i<SIDE+1;i++) {
        for (j=1;j<SIDE+1;j++) {
            if (randint(0, CHANCE + 1) != 1) {
                maze[i][j] = 1;
            }
        }
    }
    
    for (i=1;i<SIDE+1;i++) {
        for (j=1;j<SIDE+1;j++) {
            if (maze[i][j+1]) {
                graph[baris][baris+1] = 1;
            }
            if (maze[i][j-1]) {
                graph[baris][baris-1] = 1;
            }
            if (maze[i+1][j]) {
                graph[baris][baris + SIDE] = 1;
            }
            if (maze[i-1][j]) {
                graph[baris][baris - SIDE] = 1;
            }
            baris++;
        }
    }
}

void printRoute(int y, int x) {
    gotoxy((x*2) + 2, y + 2);
    if (maze[y][x] == 2) {
        printf("\033[1m\033[33m[]\033[0;37m");
    }
    else if (maze[y][x] == 4) {
        printf("\033[1m\033[33mX0\033[0;37m");
    }    
}

void visualize(int dest) {
    int row, col, i;
    char key;
    gotoxy(text_x, text_y);
    printf("Finding shortest route ... ");
    do {
        Node *curr = head[dest];
        while (curr) {
            row = curr->data / SIDE;
            col = curr->data % SIDE;
            if (maze[row+1][col+1] != 3) {
                maze[row+1][col+1] = 2;
            }
            else {
                maze[row+1][col+1] = 4;
            }
            int x = col+1;
            int y = row+1;
            printRoute(y, x);
            usleep(DELAY*1000);
            curr = curr->next;
        }

        curr = head[dest];
        while (curr) {
            row = curr->data / SIDE;
            col = curr->data % SIDE;
            if (maze[row+1][col+1] == 4) {
                maze[row+1][col+1] = 3;
            }
            if (maze[row+1][col+1] != 3) {
                maze[row+1][col+1] = 1;
            }
            curr = curr->next;
        }
        if (dist[dest] == INT_MAX) {
            gotoxy(text_x, text_y);
            printf("Destination cannot be reached! ");
            gotoxy(1, 1);
        }
        else {
            gotoxy(text_x, text_y);
            printf("DONE!                          ");
            gotoxy(text_x, text_y+1);
            printf("Distance : %d", dist[dest]);
            gotoxy(1, 1);
        }
        key = getch();
        switch (key) {
            case 'q': {
                exit(0);
            }
            case ' ': {
                printSolution(dist);
                system("pause");
                break;
            }
        }
        system("cls");
        printMaze();
    } while (key != '\r');
    for (i=0;i<SIDE*SIDE;i++) {
        pop(i);
    }
}

int menu() {
    int i, j, row, col, source_x, source_y;
    system("cls");
    randomMaze();
    do {
        printMaze();
        gotoxy(text_x, text_y);
        printf("Source (x y): ");
        scanf("%d %d", &source_x, &source_y); getchar();
        source = ((source_y-1) * SIDE) + (source_x-1);
    } while (source < 0 || source > VERTEX - 1 || !maze[source_y][source_x]);
    
    maze[source_y][source_x] = 3;
    
    dijkstra(graph, source);
    int dest, dest_x, dest_y;
    do {
        printMaze();
        gotoxy(text_x, text_y);
        printf("Destination (x y): ");
        scanf("%d %d", &dest_x, &dest_y); getchar();
        dest = ((dest_y-1) * SIDE) + (dest_x-1);
    } while (dest < 0 || dest > VERTEX - 1 || !maze[dest_y][dest_x]);
    
    maze[dest_y][dest_x] = 3;
    printMaze();
    visualize(dest);
    menu();
}
 
int main() {
    // start program in full screen mode
    keybd_event(VK_F11, 0, 0, 0);
    keybd_event(VK_F11, 0, KEYEVENTF_KEYUP, 0);

    srand(time(NULL));
    menu();
    return 0;
}
