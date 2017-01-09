#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define Max_File_Size 100000
#define K_Largest 30000

//build a max heap for the input array (in-place)
void makeHeap(int inputArray[], int inputSize);

//sift down the heap element at index index
void siftDown(int heap[], int heapSize, int index);

//sift up the heap element at index index
void siftUp(int heap[], int heapSize, int index);

void insert(int heap[], int currentHeapSize, int element);

int extractMax(int heap[], int currentHeapSize);

void printHeap(int arr[], int arrSize);

void writeOutput(int arr[], int arrSize);

int* returnKLargest(int inputArray[], int inputSize, int k, int *kLargestElements);


int main()
{
	int inputNumbers[Max_File_Size];
	FILE *fp;

	errno_t err = fopen_s(&fp, ".\\input.txt", "r");
	if (fp == NULL && err != 0)
	{
		printf("Error - cannot open input file.");
		getchar();
		exit(1);
	}

	int i = 0;
	while (fscanf_s(fp, "%d", &inputNumbers[i]) != EOF && i < Max_File_Size)
	{
		i++;
	}
	fclose(fp);

	int *kLargestElements = malloc(K_Largest * sizeof(int));
	printf("finding k largest elements...");
	clock_t begin = clock();
	writeOutput(returnKLargest(inputNumbers, i, K_Largest, kLargestElements), K_Largest);
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Done! in %f time", time_spent);

	free(kLargestElements);
	getchar();
	return 0;
}

void writeOutput(int arr[], int arrSize)
{
	FILE *fp = NULL;
	errno_t err = fopen_s(&fp, ".\\output.txt", "w+");
	if (fp == NULL && err != 0)
	{
		printf("Error - cannot open input file.");
		getchar();
		exit(1);
	}
	for (int i = 0; i < arrSize; i++)
	{
		fprintf_s(fp, "%d ", arr[i]);
	}
	fclose(fp);
}


//build a max heap for the input array (in-place)
void makeHeap(int inputArray[], int inputSize)
{
	//the level before last (from which we begin the sift downs)
	int lastLevel = (int)ceil(log(inputSize));
	int levelSize = 0;
	for (int level = lastLevel; level >= 0; level--)
	{
		levelSize = (int)pow(2, level);
		for (int i = 0; i < levelSize; i++)
			siftDown(inputArray, inputSize, i);
	}
}

//sift down the heap element at index index
void siftDown(int heap[], int heapSize, int index)
{
	//check if we have reached the bottom level
	if (2 * index + 2 > heapSize - 1)
	{
		// the current element has no children
		if (2 * index + 1 > heapSize - 1)
			return;
		// the current element only has a left child, swap them if needed
		if (heap[index] < heap[2 * index + 1])
		{
			int temp = heap[index];
			heap[index] = heap[2 * index + 1];
			heap[2 * index + 1] = temp;
		}
		return;
	}
	// if the current element is larger or equal to his children we can stop
	if (heap[index] >= heap[2 * index + 1] && heap[index] >= heap[2 * index + 2])
		return;

	// replace the current element with the larger of its children and continue sifting down
	int largerChild = heap[2 * index + 1] > heap[2 * index + 2] ? 2 * index + 1 : 2 * index + 2;
	int temp = heap[index];
	heap[index] = heap[largerChild];
	heap[largerChild] = temp;
	siftDown(heap, heapSize, largerChild);
}

//sift up the heap element at index index
void siftUp(int heap[], int heapSize, int index)
{
	//check if we have reached the root
	if (index == 0)
		return;
	int parentIndex = (int)ceil((double)index / 2) - 1;
	// if the current element is smaller or equal to his parent we can stop
	if (heap[index] <= heap[parentIndex])
		return;
	
	//replace the current element with its parent
	int temp = heap[index];
	heap[index] = heap[parentIndex];
	heap[parentIndex] = temp;
	siftUp(heap, heapSize, parentIndex);
}

void insert(int heap[], int currentHeapSize, int element)
{
	//insert the new element as the last leaf
	heap[currentHeapSize] = element;
	printHeap(heap, currentHeapSize + 1);
	siftUp(heap, currentHeapSize + 1, currentHeapSize);
}

int extractMax(int heap[], int currentHeapSize)
{
	int max = heap[0];
	//switch the root and the last leaf
	heap[0] = heap[currentHeapSize - 1];
	siftDown(heap, currentHeapSize - 1, 0);
	return max;
}

//this struct contains an element in the candidates' heap, and its index in the large heap
struct ElementIndexPair
{
	int element;
	int index;
};

//sift a candidate down in the candidates heap
void siftCandidateDown(struct ElementIndexPair candidatesHeap[], int candidatesHeapSize, int indexInCandidateHeap)
{
	//check if we have reached the bottom level
	//if the current element has no right child
	if (2 * indexInCandidateHeap + 2 > candidatesHeapSize - 1)
	{
		// the current element has no children
		if (2 * indexInCandidateHeap + 1 > candidatesHeapSize - 1)
			return;
		// the current element only has a left child, swap them if needed
		if (candidatesHeap[indexInCandidateHeap].element < candidatesHeap[2 * indexInCandidateHeap + 1].element)
		{
			struct ElementIndexPair temp = candidatesHeap[indexInCandidateHeap];
			candidatesHeap[indexInCandidateHeap] = candidatesHeap[2 * indexInCandidateHeap + 1];
			candidatesHeap[2 * indexInCandidateHeap + 1] = temp;
		}
		return;
	}
	// if the current element is larger or equal to his children we can stop
	if (candidatesHeap[indexInCandidateHeap].element >= candidatesHeap[2 * indexInCandidateHeap + 1].element && candidatesHeap[indexInCandidateHeap].element >= candidatesHeap[2 * indexInCandidateHeap + 2].element)
		return;

	// replace the current element with the larger of its children and continue sifting down
	int largerChildIndex = candidatesHeap[2 * indexInCandidateHeap + 1].element > candidatesHeap[2 * indexInCandidateHeap + 2].element ? 2 * indexInCandidateHeap + 1 : 2 * indexInCandidateHeap + 2;
	struct ElementIndexPair temp = candidatesHeap[indexInCandidateHeap];
	candidatesHeap[indexInCandidateHeap] = candidatesHeap[largerChildIndex];
	candidatesHeap[largerChildIndex] = temp;
	siftCandidateDown(candidatesHeap, candidatesHeapSize, largerChildIndex);
}

//sift a candidate up in the candidates heap
void siftCandidateUp(struct ElementIndexPair candidatesHeap[], int candidatesHeapSize, int indexInCandidateHeap)
{
	//check if we have reached the root
	if (indexInCandidateHeap == 0)
		return;
	int parentIndex = (int)ceil((double)indexInCandidateHeap / 2) - 1;
	// if the current element is smaller or equal to its parent we can stop
	if (candidatesHeap[indexInCandidateHeap].element <= candidatesHeap[parentIndex].element)
		return;

	//replace the current element with its parent
	struct ElementIndexPair temp = candidatesHeap[indexInCandidateHeap];
	candidatesHeap[indexInCandidateHeap] = candidatesHeap[parentIndex];
	candidatesHeap[parentIndex] = temp;
	siftCandidateUp(candidatesHeap, candidatesHeapSize, parentIndex);
}


//This function inserts the element into the candidates' heap, and its index to the index array
void insertToCandidateHeap(struct ElementIndexPair candidateHeap[], int currentHeapSize, struct ElementIndexPair element)
{
	//insert the new element as the last leaf
	candidateHeap[currentHeapSize] = element;
	siftCandidateUp(candidateHeap, currentHeapSize + 1, currentHeapSize);
}

int candidateHeapExtractMax(struct ElementIndexPair candidateHeap[], int currentHeapSize)
{
	int max = candidateHeap[0].element;
	//switch the root and the last leaf
	candidateHeap[0] = candidateHeap[currentHeapSize - 1];
	siftCandidateDown(candidateHeap, currentHeapSize - 1, 0);
	return max;
}



int* returnKLargest(int inputArray[], int inputSize, int k, int *kLargestElements)
{
	makeHeap(inputArray, inputSize);
	struct ElementIndexPair* candidatesHeap = malloc((k+2) * sizeof(struct ElementIndexPair));
	

	//insert the maximum to the candidates' heap
	candidatesHeap[0].element = inputArray[0];
	candidatesHeap[0].index = 0;

	for (int i = 0; i < k; i++)
	{
		//insert left child (in the large heap) of the maximum in the candidates' heap
		struct ElementIndexPair leftChild;
		leftChild.index = 2 * candidatesHeap[0].index + 1;
		leftChild.element = inputArray[leftChild.index];
		insertToCandidateHeap(candidatesHeap, i + 1, leftChild);
		//insert right child (in the large heap) of the maximum in the candidates' heap
		struct ElementIndexPair rightChild;
		rightChild.index = 2 * candidatesHeap[0].index + 2;
		rightChild.element = inputArray[rightChild.index];
		insertToCandidateHeap(candidatesHeap, i + 2, rightChild);
		//write the maximum of the candidates to the output, and extract it
		kLargestElements[i] = candidateHeapExtractMax(candidatesHeap, i + 3);
	}

	free(candidatesHeap);
	return kLargestElements;
}

void printHeap(int heap[], int heapsize)
{
	int newLevel = 0;
	int levelsLeft = (int)ceil(heapsize / 2);
	for (int i = 0; i < heapsize; i++)
	{
		if (i == newLevel)
		{
			printf("\n");
			for (int j = 0; j < levelsLeft; j++)
				printf(" ");
			newLevel = newLevel * 2 + 1;
			levelsLeft--;
		}
		printf(" %2d ", heap[i]);
	}
}