/*
 * A023133  Signature sequence of Pi (arrange the numbers i+j*x (i,j >= 1) in
 *          increasing order; the sequence of i's is the signature of x).
 * https://oeis.org/A023133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Signature sequence of Pi: a(n) is the i-coordinate of the n-th smallest value
   over all pairs (i,j) with i,j ≥ 1 ordered by increasing i + j*π.
   This implementation generates the first n values by merging the sequences
   i + j*π for each fixed j.  It uses a binary min‑heap of size n and does
   not rely on any external utilities.  The result fits in seq_t for all
   n handled here (i ≤ n).  Complexity: O(n log n). */

#include <stdlib.h>

#define PI ((long double)3.141592653589793238462643383279502884L)

typedef struct {
    long double val;
    idx_t i;
    idx_t j;
} Node;

/* swap two nodes in the heap */
static inline void heap_swap(Node *a, Node *b)
{
    Node tmp = *a;
    *a = *b;
    *b = tmp;
}

/* sift down at index idx in a heap of size sz */
static void sift_down(Node *heap, idx_t sz, idx_t idx)
{
    while (1) {
        idx_t left  = 2 * idx + 1;
        idx_t right = left + 1;
        idx_t smallest = idx;

        if (left < sz && heap[left].val < heap[smallest].val)
            smallest = left;
        if (right < sz && heap[right].val < heap[smallest].val)
            smallest = right;

        if (smallest == idx)
            break;
        heap_swap(&heap[idx], &heap[smallest]);
        idx = smallest;
    }
}

/* sift up at index idx in a heap of size sz */
static void sift_up(Node *heap, idx_t sz, idx_t idx)
{
    while (idx > 0) {
        idx_t parent = (idx - 1) >> 1;
        if (heap[idx].val >= heap[parent].val)
            break;
        heap_swap(&heap[idx], &heap[parent]);
        idx = parent;
    }
}

/* builds a min‑heap from the first sz elements of heap */
static void heapify(Node *heap, idx_t sz)
{
    for (idx_t i = (sz >> 1); i-- > 0; )
        sift_down(heap, sz, i);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* out of range, but defined as 0 */

    /* Allocate a heap holding at most n nodes.  */
    Node *heap = (Node *)malloc((size_t)n * sizeof(Node));
    if (!heap)
        return (seq_t)0;          /* allocation failure */

    /* Initial population: one element per j = 1..n, i = 1. */
    for (idx_t j = 1; j <= n; ++j) {
        heap[j-1].i   = 1;
        heap[j-1].j   = j;
        heap[j-1].val = (long double)1 + (long double)j * PI;
    }

    heapify(heap, n);

    /* Extract the smallest n-1 elements; the n-th smallest i is the answer. */
    for (idx_t k = 1; k < n; ++k) {
        Node top = heap[0];

        /* Grow i for the same j and recompute the value. */
        heap[0].i   = top.i + 1;
        heap[0].val = (long double)heap[0].i + (long double)top.j * PI;

        sift_down(heap, n, 0);
    }

    seq_t result = (seq_t)heap[0].i;
    free(heap);
    return result;
}
