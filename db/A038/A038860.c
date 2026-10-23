/*
 * A038860  Numbers ending with '5' that are the difference of two positive
 *          cubes.
 * https://oeis.org/A038860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers ending with '5' that are the difference of two positive
 *            cubes. * a(n) = the n-th smallest value of (k + d)^3 – k^3 where
 *            * d = 5(2j+1), k ≥ 1, j ≥ 0. This is equivalent to the OEIS
 *            A038860
 * Range:     u8 none, u16 none, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

typedef struct {
    seq_t val;   /* current difference for this d sequence */
    seq_t d;     /* the odd multiple of 5 (constant for the sequence) */
    idx_t k;     /* current k (positive integer) */
} Node;

/* Compute (k + d)^3 – k^3 using an integer-only formula. */
static inline seq_t diff(seq_t d, idx_t k)
{
    seq_t ksq = (seq_t)k * (seq_t)k;     /* k^2 */
    seq_t d2  = d * d;                   /* d^2 */
    seq_t term = d * ((seq_t)3 * ksq +
                      (seq_t)3 * (seq_t)k * d +
                      d2);
    return term;
}

/* Sift a node down the heap until the min‑heap property holds. */
static void sift_down(int i, Node *h, int sz)
{
    while (1) {
        int left  = 2 * i + 1;
        int right = left + 1;
        int smallest = i;

        if (left < sz && h[left].val < h[smallest].val)
            smallest = left;
        if (right < sz && h[right].val < h[smallest].val)
            smallest = right;

        if (smallest == i)
            break;
        Node tmp = h[i];
        h[i] = h[smallest];
        h[smallest] = tmp;
        i = smallest;
    }
}

/* Sift a node up the heap until the min‑heap property holds. */
static void sift_up(int i, Node *h, int sz)
{
    while (i > 0) {
        int parent = (i - 1) >> 1;
        if (h[i].val >= h[parent].val)
            break;
        Node tmp = h[i];
        h[i] = h[parent];
        h[parent] = tmp;
        i = parent;
    }
}

/* Return the n-th term of A038860 (1‑based index). */
seq_t a(idx_t n)
{
    const idx_t D_MAX = 2000;            /* number of d‑sequences to merge */
    Node heap[D_MAX];
    int heap_size = (int)D_MAX;          /* heap size is kept constant */

    /* Initialize all d‑sequences with k = 1. */
    for (idx_t j = 0; j < D_MAX; ++j) {
        seq_t d = (seq_t)5 * ( (seq_t)2 * j + 1 );
        heap[j].d = d;
        heap[j].k = 1;
        heap[j].val = diff(d, 1);
    }

    /* Build the min‑heap. */
    for (int i = (heap_size >> 1) - 1; i >= 0; --i)
        sift_down(i, heap, heap_size);

    seq_t prev = 0;          /* previous value output (for duplicate suppression) */
    idx_t count = 0;         /* number of distinct values emitted */

    while (1) {
        /* Top of the heap is the smallest next candidate. */
        Node top = heap[0];
        seq_t val = top.val;

        /* Remove the root. */
        heap[0] = heap[--heap_size];
        sift_down(0, heap, heap_size);

        /* Emit if not a duplicate. */
        if (val != prev) {
            prev = val;
            ++count;
            if (count == n)
                return val;
        }

        /* Advance the sequence that produced this value. */
        ++top.k;
        top.val = diff(top.d, top.k);

        /* Re‑insert the updated node. */
        heap[heap_size++] = top;
        sift_up(heap_size - 1, heap, heap_size);
    }
}
