/*
 * A023126  Signature sequence of 1/e^2 (arrange the numbers i+j*x (i,j >= 1)
 *          in increasing order; the sequence of i's is the signature of x).
 * https://oeis.org/A023126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the i‑coordinate of the nth smallest value of i +
 *            j*(1/e²) with i, j ≥ 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a min‑heap that always keeps, for each
   i ≤ n, the smallest not yet extracted value with that i.  The heap
   size is n, which is sufficient because the first n values all have
   i ≤ n.  The weight of a pair (i, j) is i*SCALE + j*XVAL where
   SCALE = 10¹⁸ and XVAL = floor((1/e²)*10¹⁸).  The weight is stored
   in unsigned __int128 to avoid overflow. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Constants for scaling the irrational number x = 1/e². */
static const unsigned long long SCALE = 1000000000000000000ULL;          /* 10¹⁸ */
static const unsigned long long XVAL  = 135335283236612691ULL;          /* floor(x*SCALE) */

/* Node used in the min‑heap: i, current j, and the weight. */
typedef struct {
    idx_t     i;
    idx_t     j;
    unsigned __int128 weight;
} Node;

/* Compare two heap nodes: primarily by weight, secondarily by i. */
static inline int node_cmp(const Node *a, const Node *b) {
    if (a->weight < b->weight) return -1;
    if (a->weight > b->weight) return  1;
    if (a->i < b->i)            return -1;
    if (a->i > b->i)            return  1;
    return 0;
}

/* Sift the node at idx downwards in a min‑heap of size heap_sz. */
static void sift_down(Node *heap, size_t heap_sz, size_t idx) {
    while (1) {
        size_t left = idx * 2 + 1;
        if (left >= heap_sz) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_sz && node_cmp(&heap[right], &heap[left]) < 0)
            smallest = right;
        if (node_cmp(&heap[smallest], &heap[idx]) >= 0) break;
        Node tmp = heap[idx];
        heap[idx] = heap[smallest];
        heap[smallest] = tmp;
        idx = smallest;
    }
}

/* Return the nth term of the signature sequence (1‑based index). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for offset 1 */

    /* Allocate a heap capable of holding at most n distinct i‑values. */
    Node *heap = (Node *)malloc((size_t)n * sizeof(Node));
    if (!heap) return (seq_t)0;            /* allocation failure */

    /* Initialise each heap entry with j = 1. */
    for (idx_t i = 1; i <= n; ++i) {
        heap[i - 1].i      = i;
        heap[i - 1].j      = 1ULL;
        heap[i - 1].weight = (unsigned __int128)i * SCALE
                            + (unsigned __int128)1ULL * XVAL;
    }

    /* Build a min‑heap from the initial array. */
    for (size_t k = n / 2; k > 0; --k) {
        sift_down(heap, (size_t)n, k - 1);
    }

    seq_t result = 0;

    /* Extract the smallest weight n times, each time outputting its i. */
    for (idx_t step = 0; step < n; ++step) {
        Node top = heap[0];
        result = (seq_t)top.i;                /* nth term */

        /* Advance j for this i and recompute its weight. */
        ++top.j;
        top.weight = (unsigned __int128)top.i * SCALE
                   + (unsigned __int128)top.j * XVAL;
        heap[0] = top;

        sift_down(heap, (size_t)n, 0);
    }

    free(heap);
    return result;
}
