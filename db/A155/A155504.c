/*
 * A155504  Numbers of the form (3h+1)*3^(k+1) listed in increasing order.
 * https://oeis.org/A155504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers of the form (3h+1)*3^(k+1) listed in increasing order
 *            (OEIS A155504)
 * Range:     u8 n <= 44, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by a priority‑queue over the arithmetic progressions indexed by k. */

#include <stdlib.h>
#include <stdint.h>

typedef struct {
    idx_t val;   /* current smallest element of this progression */
    idx_t step;  /* fixed increment  = 3^(k+2) */
} Node;

/* swap two nodes */
static void swap_node(Node *a, Node *b)
{
    Node tmp = *a; *a = *b; *b = tmp;
}

/* heapify down from index i in a heap of given size */
static void heapify_down(Node *heap, size_t size, size_t i)
{
    while (1) {
        size_t left  = 2 * i + 1;
        size_t right = left + 1;
        size_t smallest = i;

        if (left < size && heap[left].val < heap[smallest].val)
            smallest = left;
        if (right < size && heap[right].val < heap[smallest].val)
            smallest = right;

        if (smallest == i)
            break;

        swap_node(&heap[i], &heap[smallest]);
        i = smallest;
    }
}

/* build a min‑heap from an array */
static void build_heap(Node *heap, size_t size)
{
    if (size <= 1) return;
    for (size_t i = (size >> 1); i-- > 0; )
        heapify_down(heap, size, i);
}

/* return the n‑th term of A155504 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0 */

    /* A rough upper bound on a(n) – guarantees we generate enough terms. */
    idx_t limit;
    if (n > UINT64_MAX/10)           /* avoid overflow of n*10 */
        limit = UINT64_MAX;
    else
        limit = n * 10;
    limit += 10;                     /* a safety margin */

    /* Compute powers of 3 up to this bound.  We work in idx_t to avoid overflow
       even if seq_t is a small (8‑ or 16‑bit) type. */
    size_t pow_count = 0;           /* number of powers of 3 ≤ limit (including 3^0) */
    idx_t pow = 1;
    while (pow <= limit) {
        pow_count++;
        if (pow > limit / 3) break;
        pow *= 3;
    }

    /* The largest k needed satisfies 3^(k+1) ≤ limit.
       Number of such k is (pow_count - 1) - 1 = pow_count - 2 if pow_count ≥ 2. */
    size_t tmax = (pow_count >= 2) ? pow_count - 2 : 0;   /* k from 0 .. tmax inclusive */

    /* Allocate array of the necessary powers of 3. */
    size_t pow_size = pow_count + 2;      /* need up to index (pow_count+1) */
    idx_t *pow3 = (idx_t *)malloc(pow_size * sizeof(idx_t));
    if (!pow3) return 0;                                /* memory error */
    pow3[0] = 1;
    for (size_t i = 1; i <= pow_count + 1; ++i)
        pow3[i] = pow3[i-1] * (idx_t)3;

    /* Allocate and initialise the heap of progressions. */
    size_t heap_size = tmax + 1;             /* number of progressions */
    Node *heap = (Node *)malloc(heap_size * sizeof(Node));
    if (!heap) { free(pow3); return 0; }
    for (size_t k = 0; k <= tmax; ++k) {
        heap[k].val  = pow3[k + 1];     /* 3^(k+1)   */
        heap[k].step = pow3[k + 2];     /* 3^(k+2)   */
    }
    build_heap(heap, heap_size);

    /* Extract the terms in order, skipping duplicates (which cannot occur
       but the code is robust). */
    size_t extracted = 0;
    seq_t result = 0;
    idx_t prev_val = 0;
    int has_prev = 0;

    while (extracted < n) {
        /* Smallest current term is at heap[0]. */
        idx_t cur = heap[0].val;

        /* Output the term only if it differs from the previous one. */
        if (!has_prev || cur != prev_val) {
            result = (seq_t)cur;
            extracted++;
            prev_val = cur;
            has_prev = 1;
        }

        /* Advance this progression and restore the heap property. */
        heap[0].val += heap[0].step;
        heapify_down(heap, heap_size, 0);
    }

    free(heap);
    free(pow3);
    return result;
}
