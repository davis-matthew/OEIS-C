/*
 * A023134  Signature sequence of 1/Pi (arrange the numbers i+j*x (i,j >= 1)
 *          in increasing order; the sequence of i's is the signature of x).
 * https://oeis.org/A023134
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

/* Computes the signature sequence of 1/π (OEIS A023134) by merging
   the infinite sequences  i + j/π  for each i = 1 .. n.  The heap
   keeps the next candidate from each sequence. */
#include <stdlib.h>
#include <stdint.h>

/* Internal node in the min‑heap */
struct Node {
    int i;           /* first coordinate */
    int j;           /* second coordinate (starting from 1) */
    double val;      /* current value i + j/π */
};

/* Minimum element propagation downwards in heap */
static void sift_down(struct Node *heap, size_t n, size_t idx) {
    while (idx * 2 <= n) {
        size_t child = idx * 2;
        if (child + 1 <= n && heap[child + 1].val < heap[child].val)
            child++;
        if (heap[child].val < heap[idx].val) {
            struct Node tmp = heap[child];
            heap[child] = heap[idx];
            heap[idx] = tmp;
            idx = child;
        } else
            break;
    }
}

/* External function required by the build system */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* offset starts at 1 */

    /* Allocate heap of size min(n, 100000) to avoid excessive memory */
    size_t heap_size = (size_t)n;
    if (heap_size == 0) return (seq_t)0;

    struct Node *heap = (struct Node *)malloc((heap_size + 1) * sizeof(struct Node));
    if (!heap) return (seq_t)0;   /* allocation failure */

    const double invpi = 1.0 / 3.14159265358979323846;   /* 1/π */

    /* Initialize heap with one element per i (j=1) */
    for (size_t ii = 1; ii <= heap_size; ++ii) {
        heap[ii].i = (int)ii;
        heap[ii].j = 1;
        heap[ii].val = (double)heap[ii].i + heap[ii].j * invpi;
        /* sift up to maintain heap property after insertion */
        size_t child = ii;
        while (child > 1) {
            size_t parent = child >> 1;
            if (heap[child].val < heap[parent].val) {
                struct Node tmp = heap[child];
                heap[child] = heap[parent];
                heap[parent] = tmp;
                child = parent;
            } else
                break;
        }
    }

    seq_t result = (seq_t)0;

    /* Produce the first n terms by repeatedly using the smallest item. */
    for (size_t step = 0; step < (size_t)n; ++step) {
        /* Smallest element is at the root of the heap. */
        result = (seq_t)heap[1].i;

        /* Advance the j index for this i and recompute its value. */
        heap[1].j++;
        heap[1].val = (double)heap[1].i + heap[1].j * invpi;

        /* Restore heap invariant. */
        sift_down(heap, heap_size, 1);
    }

    free(heap);
    return result;
}
