/*
 * A174069  Numbers that can be written as a sum of at least 2 squares of
 *          consecutive positive integers.
 * https://oeis.org/A174069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 108, u32 n <= 931, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sumConsecutiveSquares, sumSquares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A174069: numbers that can be written as a sum of at least two
   squares of consecutive positive integers.  The algorithm generates
   all such sums in ascending order using a fixed‑size min‑heap. */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/sumConsecutiveSquares.h"
#include "../utils/sumSquares.h"

/* ----- heap node ----------------------------------------------------- */
typedef struct {
    seq_t val;   /* current sum */
    idx_t m;     /* length of block of consecutive squares */
    idx_t k;     /* start index (≥ 1) */
} Node;

/* ----- swap two nodes ----------------------------------------------- */
static inline void swapNode(Node *a, Node *b) {
    Node t = *a; *a = *b; *b = t;
}

/* ----- sift down in a min‑heap ---------------------------------------- */
static inline void siftDown(Node *h, size_t i, size_t heapSize) {
    while (1) {
        size_t left  = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;
        if (left < heapSize && h[left].val < h[smallest].val)
            smallest = left;
        if (right < heapSize && h[right].val < h[smallest].val)
            smallest = right;
        if (smallest == i) break;
        swapNode(&h[i], &h[smallest]);
        i = smallest;
    }
}

/* ----- heapify the array --------------------------------------------- */
static void heapify(Node *h, size_t heapSize) {
    for (size_t i = heapSize / 2; i > 0; --i)
        siftDown(h, i - 1, heapSize);
}

/* ----- main sequence function ---------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;    /* undefined for n==0, return sentinel */

    /* Number of distinct starting m values to pre‑allocate.
       For n up to about 10 000 this guarantees enough elements.      */
    idx_t max_m = n * 2;
    if (max_m < 4) max_m = 4;   /* ensure at least m = 2..3 are present */

    size_t heapSize = (size_t)(max_m - 1);   /* m runs from 2 to max_m inclusive */
    Node *heap = (Node *)malloc(heapSize * sizeof(Node));
    if (!heap) return 0;   /* allocation failure */

    /* initialise heap nodes : one per m, with starting k = 1          */
    for (idx_t m = 2; m <= max_m; ++m) {
        heap[m - 2].m = m;
        heap[m - 2].k = 1;
        heap[m - 2].val = sumConsecutiveSquares(1, m);
    }

    heapify(heap, heapSize);

    seq_t prev = 0;
    size_t count = 0;

    /* extract smallest sums in ascending order, skipping duplicates   */
    while (count < n) {
        Node cur = heap[0];

        if (prev != cur.val) {
            prev = cur.val;
            ++count;
            if (count == n) {
                free(heap);
                return prev;
            }
        }

        /* advance to the next sum for the same m */
        ++cur.k;
        cur.val = sumConsecutiveSquares(cur.k, cur.m);
        heap[0] = cur;

        siftDown(heap, 0, heapSize);
    }

    /* unreachable */
    free(heap);
    return 0;
}
