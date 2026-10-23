/*
 * A126237  Length of row n in table A126014.
 * https://oeis.org/A126237
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = (number of distinct Huffman codeword lengths for weights 1..n) - 1
   The function constructs the Huffman tree for the weights 1,2,…,n,
   records the depth of each leaf, counts how many different depths
   occur, and returns that count minus one.  It works for any seq_t
   and idx_t provided by the build.
*/
#include <stdlib.h>
#include <stdbool.h>

/* Node used to build the Huffman tree */
struct Node {
    seq_t weight;
    int   left;   /* -1 for a leaf */
    int   right;  /* -1 for a leaf */
};

/* -------------------------------------------------------------------- */
/* Binary min‑heap helpers                                               */
/* -------------------------------------------------------------------- */
static inline void sift_down(size_t pos, size_t size, struct Node *nodes,
                             size_t *heap)
{
    for (;;) {
        size_t smallest = pos;
        size_t left    = pos * 2 + 1;
        size_t right   = left + 1;

        if (left < size &&
            nodes[heap[left]].weight  < nodes[heap[smallest]].weight)
            smallest = left;
        if (right < size &&
            nodes[heap[right]].weight < nodes[heap[smallest]].weight)
            smallest = right;

        if (smallest == pos)
            break;

        size_t tmp = heap[pos];
        heap[pos]  = heap[smallest];
        heap[smallest] = tmp;
        pos = smallest;
    }
}

static inline void sift_up(size_t pos, struct Node *nodes, size_t *heap)
{
    for (;;) {
        if (pos == 0)
            break;
        size_t parent = (pos - 1) >> 1;
        if (nodes[heap[pos]].weight < nodes[heap[parent]].weight) {
            size_t tmp = heap[pos];
            heap[pos]  = heap[parent];
            heap[parent] = tmp;
            pos = parent;
        } else
            break;
    }
}

/* -------------------------------------------------------------------- */
/* Main sequence function                                                */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 3) {
        /* Sequence is undefined for n < 3; return 0 to avoid UB. */
        return 0;
    }

    size_t N = (size_t)n;                /* number of leaves */
    size_t totalNodes = 2 * N - 1;       /* leaves + internal nodes */

    /* Allocate nodes and initialise leaves. */
    struct Node *nodes = (struct Node *)calloc(totalNodes, sizeof(struct Node));
    if (!nodes)
        return 0;                        /* allocation failure guard */

    for (size_t i = 0; i < N; ++i) {
        nodes[i].weight = (seq_t)(i + 1);
        nodes[i].left   = -1;
        nodes[i].right  = -1;
    }

    /* Allocate heap that initially contains all leaf indices. */
    size_t *heap = (size_t *)calloc(N, sizeof(size_t));
    if (!heap) {
        free(nodes);
        return 0;
    }
    for (size_t i = 0; i < N; ++i)
        heap[i] = i;

    /* Build the initial min‑heap. */
    for (int i = (int)(N / 2) - 1; i >= 0; --i)
        sift_down((size_t)i, N, nodes, heap);

    size_t heapSize = N;       /* current heap size */
    size_t nextIdx   = N;      /* index of next internal node */

    /* Huffman merge loop: keep merging until one node remains. */
    while (heapSize > 1) {
        /* Pop two smallest nodes. */
        size_t idx1 = heap[0];
        heap[0] = heap[--heapSize];
        sift_down(0, heapSize, nodes, heap);

        size_t idx2 = heap[0];
        heap[0] = heap[--heapSize];
        sift_down(0, heapSize, nodes, heap);

        /* Create a new internal node. */
        nodes[nextIdx].weight = nodes[idx1].weight + nodes[idx2].weight;
        nodes[nextIdx].left   = (int)idx1;
        nodes[nextIdx].right  = (int)idx2;

        /* Push the new node onto the heap. */
        heap[heapSize++] = nextIdx;
        sift_up(heapSize - 1, nodes, heap);

        ++nextIdx;
    }

    /* Root of the tree is heap[0].  We now compute leaf depths. */
    int *leafDepth = (int *)calloc(N, sizeof(int));
    if (!leafDepth) {
        free(heap);
        free(nodes);
        return 0;
    }

    /* Stack for iterative DFS. */
    size_t stackCap = N + 10;
    size_t *stackNode = (size_t *)calloc(stackCap, sizeof(size_t));
    int   *stackDepth = (int *)calloc(stackCap, sizeof(int));
    if (!stackNode || !stackDepth) {
        free(heap);
        free(nodes);
        free(leafDepth);
        free(stackNode);
        free(stackDepth);
        return 0;
    }

    size_t sp = 0;
    stackNode[sp]   = heap[0];   /* root index */
    stackDepth[sp]  = 0;
    ++sp;

    while (sp) {
        --sp;
        size_t idx = stackNode[sp];
        int d      = stackDepth[sp];

        if (nodes[idx].left == -1) {          /* leaf */
            leafDepth[(int)idx] = d;
        } else {                              /* internal node */
            /* Push left child. */
            stackNode[sp]  = (size_t)nodes[idx].left;
            stackDepth[sp] = d + 1;
            ++sp;
            /* Push right child. */
            stackNode[sp]  = (size_t)nodes[idx].right;
            stackDepth[sp] = d + 1;
            ++sp;
        }
    }

    /* Count distinct depths. */
    bool *seen = (bool *)calloc(N + 1, sizeof(bool)); /* depth cannot exceed N */
    if (!seen) {
        free(heap);
        free(nodes);
        free(leafDepth);
        free(stackNode);
        free(stackDepth);
        return 0;
    }

    int distinct = 0;
    for (size_t i = 0; i < N; ++i) {
        int d = leafDepth[i];
        if (!seen[d]) {
            seen[d] = true;
            ++distinct;
        }
    }

    seq_t result = (seq_t)distinct - (seq_t)1;

    /* Clean up. */
    free(heap);
    free(nodes);
    free(leafDepth);
    free(stackNode);
    free(stackDepth);
    free(seen);

    return result;
}
