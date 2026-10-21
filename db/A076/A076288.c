/*
 * A076288  Numbers of the form (9^{mr}-1)/(9^r-1) for positive integers m, r.
 * https://oeis.org/A076288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of the form (9^{mr}-1)/(9^r-1) for
 * positive integers m, r, sorted ascending and unique.
 * The function generates the sequence using a min‑heap over all
 * sequences r.  For each r > 0 the values are:
 *   v_1 = 9^r + 1
 *   v_{k+1} = v_k * 9^r + 1          (k ≥ 1)
 *
 * The algorithm maintains the next value of each active r in a
 * binary min‑heap.  The heap size never exceeds the number of
 * distinct r values needed to reach the requested index n
 * (≈ 1400 for n = 10000), so a fixed array suffices both in C and
 * in C++.  No external libraries are required beyond the standard
 * integer types provided by the build system. */
#include <stddef.h>

typedef struct {
    seq_t val;   /* next value of this sequence */
    seq_t a;     /* 9^r for this sequence */
    unsigned r;  /* r-value (only for bookkeeping) */
} node_t;

/* push a node into a binary min‑heap */
static void heap_push(node_t *h, size_t *sz, node_t n)
{
    size_t i = *sz;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (h[p].val <= n.val) break;
        h[i] = h[p];
        i = p;
    }
    h[i] = n;
    (*sz)++;
}

/* pop the minimum element from the heap */
static node_t heap_pop(node_t *h, size_t *sz)
{
    node_t res = h[0];
    node_t last = h[--(*sz)];
    if (*sz > 0) {
        size_t i = 0;
        while (1) {
            size_t l = (i << 1) + 1;
            if (l >= *sz) break;
            size_t r = l + 1;
            size_t c = (r < *sz && h[r].val < h[l].val) ? r : l;
            if (last.val <= h[c].val) break;
            h[i] = h[c];
            i = c;
        }
        h[i] = last;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;      /* first element is 1 */

    /* maximum number of distinct r values that can be needed for
     * n up to 10000 (≈1400).  A small safety margin is added. */
    enum { MAX_HEAP = 2500 };

    node_t heap[MAX_HEAP];
    size_t heap_sz = 0;

    seq_t pow9 = (seq_t)9;   /* 9^1 */
    size_t next_r = 2;       /* first r not yet inserted */

    /* insert the sequence for r = 1 (m ≥ 2) */
    heap_push(heap, &heap_sz, (node_t){pow9 + (seq_t)1, pow9, 1});

    size_t count = 1;        /* we have produced a(1)=1 so far */

    while (1) {
        /* lazily insert new r sequences whose first value is
         * not larger than the current minimum in the heap. */
        while (next_r <= MAX_HEAP && (heap_sz == 0 ||
               ((pow9 * (seq_t)9) + (seq_t)1) <= heap[0].val)) {
            seq_t a_next = pow9 * (seq_t)9;            /* 9^next_r */
            seq_t val_next = a_next + (seq_t)1;        /* 9^r + 1   */
            heap_push(heap, &heap_sz, (node_t){val_next, a_next, (unsigned)next_r});
            pow9 = a_next;                            /* update for next r */
            next_r++;
        }

        /* pop the current smallest term */
        node_t cur = heap_pop(heap, &heap_sz);
        ++count;
        if (count == n) return cur.val;

        /* push the next element of this r‑sequence */
        seq_t next_val = cur.val * cur.a + (seq_t)1;
        heap_push(heap, &heap_sz, (node_t){next_val, cur.a, cur.r});
    }
}
