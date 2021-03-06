#include <iostream>
#include <limits.h>
using namespace std;

// A class to represent a node in adjaceny list
class AdjListNode{
    public:
        int dest;
        int time;
        int price;
        AdjListNode* next;
};

// A node to represent an adjacency list
class AdjList{
    public:
        AdjListNode* head;
};

// A class to represent a graph. A graph is an array of adjaceny lists.
// Size of array will be V (# of vertices in graph)
class Graph{
    public:
        int V;
        AdjList* array;
};

// A class to represent a min heap node 
class MinHeapNode{ 
    public:
        int  v; 
        int dist; 
};

// A class to represent a min heap 
class MinHeap{ 
    public:
        int size;       // Number of heap nodes present currently 
        int capacity;   // Capacity of min heap 
        int *pos;       // This is needed for decreaseKey() 
        MinHeapNode **array; 
};

// A utility function to create a new adjacency list node 
AdjListNode* newAdjListNode(int dest, int price, int time){ 
    AdjListNode* newNode = (AdjListNode*) malloc(sizeof(AdjListNode)); 
    newNode->dest = dest; 
    newNode->price = price;
    newNode->time = time; 
    newNode->next = NULL; 
    return newNode; 
}

// A utility function that creates a graph of V vertices 
Graph* createGraph(int V){ 
    Graph* graph = (Graph*) malloc(sizeof(Graph)); 
    graph->V = V; 
  
    // Create an array of adjacency lists.  Size of array will be V 
    graph->array = (AdjList*) malloc(V * sizeof(AdjList)); 
  
     // Initialize each adjacency list as empty by making head as NULL 
    for (int i = 0; i < V; ++i) 
        graph->array[i].head = NULL; 
  
    return graph; 
}

// Adds an edge to an undirected graph 
void addEdge( Graph* graph, int src, int dest, int time, int price){ 
    // Add an edge from src to dest.  
    // A new node is added to the adjacency list of src.
    //  The node is added at the beginning .
    AdjListNode* newNode = newAdjListNode(dest, price, time); 
    newNode->next = graph->array[src].head; 
    graph->array[src].head = newNode; 
  
    // Since graph is undirected, add an edge from dest to src also 
    newNode = newAdjListNode(src, price, time); 
    newNode->next = graph->array[dest].head; 
    graph->array[dest].head = newNode; 
} 

// A utility function to create a new Min Heap Node 
MinHeapNode* newMinHeapNode(int v, int dist){ 
    MinHeapNode* minHeapNode = (MinHeapNode*) malloc(sizeof( MinHeapNode)); 
    minHeapNode->v = v; 
    minHeapNode->dist = dist; 
    return minHeapNode; 
} 

// A utility function to create a Min Heap 
MinHeap* createMinHeap(int capacity){ 
    MinHeap* minHeap = (MinHeap*) malloc(sizeof(MinHeap)); 
    minHeap->pos = (int *)malloc(capacity * sizeof(int)); 
    minHeap->size = 0; 
    minHeap->capacity = capacity; 
    minHeap->array = ( MinHeapNode**) malloc(capacity * sizeof( MinHeapNode*)); 
    return minHeap; 
} 
  
// A utility function to swap two nodes of min heap. Needed for min heapify 
void swapMinHeapNode( MinHeapNode** a, MinHeapNode** b){ 
    MinHeapNode* t = *a; 
    *a = *b; 
    *b = t; 
}

// A standard function to heapify at given idx 
// This function also updates position of nodes when they are swapped. 
// Position is needed for decreaseKey() 
void minHeapify(MinHeap* minHeap, int idx){ 
    int smallest, left, right; 
    smallest = idx; 
    left = 2 * idx + 1; 
    right = 2 * idx + 2; 
  
    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist ){
        smallest = left; 
    }
  
    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist ){
        smallest = right;
    } 
  
    if (smallest != idx){ 
        // The nodes to be swapped in min heap 
        MinHeapNode *smallestNode = minHeap->array[smallest]; 
        MinHeapNode *idxNode = minHeap->array[idx]; 
  
        // Swap positions 
        minHeap->pos[smallestNode->v] = idx; 
        minHeap->pos[idxNode->v] = smallest; 
  
        // Swap nodes 
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]); 
  
        minHeapify(minHeap, smallest); 
    } 
} 

// A utility function to check if the given minHeap is empty or not 
int isEmpty(MinHeap* minHeap){ 
    return minHeap->size == 0; 
}

// Standard function to extract minimum node from heap 
MinHeapNode* extractMin( MinHeap* minHeap){ 
    if (isEmpty(minHeap)){
        return NULL;
    } 
  
    // Store the root node 
    MinHeapNode* root = minHeap->array[0]; 
  
    // Replace root node with last node 
    MinHeapNode* lastNode = minHeap->array[minHeap->size - 1]; 
    minHeap->array[0] = lastNode; 
  
    // Update position of last node 
    minHeap->pos[root->v] = minHeap->size-1; 
    minHeap->pos[lastNode->v] = 0; 
  
    // Reduce heap size and heapify root 
    --minHeap->size; 
    minHeapify(minHeap, 0); 
  
    return root; 
}

// Function to decreasy dist value of a given vertex v. This function 
// uses pos[] of min heap to get the current index of node in min heap 
void decreaseKey(MinHeap* minHeap, int v, int dist){ 
     
    int i = minHeap->pos[v];        // Get the index of v in  heap array
    minHeap->array[i]->dist = dist; // Get the node and update its dist value 
  
    // Travel up while the complete tree is not hepified. 
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist){ 
        // Swap this node with its parent 
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2; 
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i; 
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]); 
        i = (i - 1) / 2; // move to parent index 
    } 
} 

// A utility function to check if a given vertex, 'v' is in min heap or not 
bool isInMinHeap( MinHeap *minHeap, int v){ 
   if (minHeap->pos[v] < minHeap->size){
       return true; 
   }
   return false; 
} 

// Function to print shortest path from source to j using parent array
void printPath(string* city_arr, int parent[], int j){
    // Base Case : If j is source
    if (parent[j]==-1){
        return;
    }
    printPath(city_arr, parent, parent[j]);
    cout << " -> " << city_arr[j];
}

// A utility function to print the constructed distance array
int printSolution(string* city_arr, int src, int destination, int dist[], int n, int parent[], int cost[], char offerType){
    cout << "Departure City\t";
    cout <<"Arrival City\t";
    cout <<"Duration\t";
    cout <<"Price\t\t";
    cout <<"Path"<<endl;
   
    cout << city_arr[src] << "\t\t";
    cout << city_arr[destination] << "\t\t";
    if(offerType == 'P'){  
        cout << cost[destination] << "\t\t";
        cout << dist[destination] << "\t\t"; 
    }
    if(offerType == 'T'){  
        cout << dist[destination] << "\t\t";
        cout << cost[destination] << "\t\t"; 
    }
    cout << city_arr[src];

    printPath(city_arr, parent, destination);
    cout << endl;
}
// The main function that calulates distances of shortest paths from src to all vertices.
// It is a O(ELogV) function 
void dijkstra(Graph* graph, string* city_arr, int src, int destination, char offerType) { 
    int V = graph->V;   // Get the number of vertices in graph 
    int dist[V];        // dist values used to pick minimum price edge in cut 
    int cost[V]; 
    int parent[V]; 
    MinHeap* minHeap = createMinHeap(V);    // minHeap represents set E 
  
    // Initialize min heap with all vertices. dist value of all vertices  
    for (int v = 0; v < V; ++v){ 
        parent[v] = -1; 
        cost[v] = 0;
        dist[v] = INT_MAX; 
        minHeap->array[v] = newMinHeapNode(v, dist[v]); 
        minHeap->pos[v] = v; 
    } 
  
    // Make dist value of src vertex as 0 so that it is extracted first 
    minHeap->array[src] = newMinHeapNode(src, dist[src]); 
    minHeap->pos[src]   = src; 
    dist[src] = 0;     
    decreaseKey(minHeap, src, dist[src]); 
    cost[src] = dist[src];
  
    minHeap->size = V;  // Initially size of min heap is equal to V 
  
    // In the followin loop, min heap contains all nodes whose shortest distance is not yet finalized. 
    while (!isEmpty(minHeap)){ 
        MinHeapNode* minHeapNode = extractMin(minHeap);     // Extract the vertex with minimum distance value 
        int u = minHeapNode->v;                             // Store the extracted vertex number 
  
        
        AdjListNode* pCrawl = graph->array[u].head;         // Traverse through all adjacent vertices of u (the extracted vertex) and update their distance values 
        while (pCrawl != NULL){ 
            int v = pCrawl->dest; 
            // If shortest distance to v is not finalized yet, and distance to v through u is less than its previously calculated distance 
            if (offerType == 'P' && isInMinHeap(minHeap, v) && dist[u] != INT_MAX && pCrawl->price + dist[u] < dist[v]){ 
                dist[v] = dist[u] + pCrawl->price; 
                cost[v] = cost[u] + pCrawl->time;
                parent[v] = u;                    // update the parent array
                decreaseKey(minHeap, v, dist[v]); // update distance value in min heap also 
            } 
            if (offerType == 'T' && isInMinHeap(minHeap, v) && dist[u] != INT_MAX && pCrawl->time + dist[u] < dist[v]){ 
                dist[v] = dist[u] + pCrawl->time; 
                cost[v] = cost[u] + pCrawl->price; 
                parent[v] = u;                    // update the parent array
                decreaseKey(minHeap, v, dist[v]); // update distance value in min heap also 
            } 
            pCrawl = pCrawl->next; 
        } 
    } 
    
    printSolution(city_arr, src, destination, dist, V, parent, cost , offerType);   // print the optimum route and some informations
} 

bool citiesValid(string source, string destination, string* city_arr, int v){
    bool src_valid  = false;
    bool dest_valid = false;

    for (int i = 0; i < v; i++){
        if(source == city_arr[i])
            src_valid = true;
        if(destination == city_arr[i])
            dest_valid = true;
        if(src_valid && dest_valid) break;
    }

    return (src_valid && dest_valid);
}

int getIndexOfCity(string city, string* city_arr, int v){
    for(int i = 0; i< v; i++){
        if(city == city_arr[i])
            return i;
    }
}
int main(){
    int V = 9;      // number of cities
    string cityList [] ={"Bodrum", "Ankara", "Istanbul", "Amsterdam", "Barcelona", "Paris", "London", "Roma", "Helsinki"};
    
    int source, destination;
    string departure, arrival;

    cout << "Please Enter the Departure City\t: " ;     cin >> departure;
    cout << "Please Enter the Arrival City\t: " ;       cin >> arrival;
    

    if(!citiesValid(departure, arrival, cityList, V)){   // City control if arrival and departure is in city array or not
        cout << "NO FLIGHTS!!! " << endl;
    }
    else{
        destination = getIndexOfCity(arrival, cityList, V);
        source      = getIndexOfCity(departure, cityList, V);

        Graph* graph = createGraph(V); 
        addEdge(graph, 0, 1, 4, 300);   // graph , source, destination, duration, price
        addEdge(graph, 0, 7, 8, 50); 
        addEdge(graph, 1, 2, 8, 30); 
        addEdge(graph, 1, 7, 11, 100); 
        addEdge(graph, 2, 3, 7, 200); 
        addEdge(graph, 2, 8, 2, 50); 
        addEdge(graph, 2, 5, 4, 300); 
        addEdge(graph, 3, 4, 9, 120); 
        addEdge(graph, 3, 5, 14, 30); 
        addEdge(graph, 4, 5, 10, 140); 
        addEdge(graph, 5, 6, 2, 700); 
        addEdge(graph, 6, 7, 1, 1400); 
        addEdge(graph, 6, 8, 6, 600); 
        addEdge(graph, 7, 8, 7, 70); 
        cout<<endl;
        // cout << "\n\t\tOptimum Route For PRICE" << endl;
        dijkstra(graph, cityList, source, destination, 'P'); 
        // cout << "\n\t\tOptimum Route For TIME" << endl;
        cout<<endl;
        dijkstra(graph, cityList, source, destination, 'T'); 
    }
    return 0;
}