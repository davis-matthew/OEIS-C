/*
 * A100685  Powers of factorials A000142.
 * https://oeis.org/A100685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = all numbers of the form (k!)^m (k ≥ 1, m ≥ 1), sorted
 *            strictly increasing
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow2.h"

typedef struct { seq_t value; unsigned int idx; } Node;

/* Min‑heap push */
static void heap_push(Node *h, size_t *sz, Node n) {
    size_t i = (*sz)++;
    while (i > 0) {
        size_t p = (i - 1) / 2;
        if (h[p].value <= n.value) break;
        h[i] = h[p];
        i = p;
    }
    h[i] = n;
}

/* Min‑heap pop (removes and returns the smallest element) */
static Node heap_pop(Node *h, size_t *sz) {
    Node res = h[0];
    Node last = h[--(*sz)];
    size_t i = 0;
    while (1) {
        size_t l = i * 2 + 1;
        if (l >= *sz) break;
        size_t r = l + 1;
        size_t best = l;
        if (r < *sz && h[r].value < h[l].value) best = r;
        if (last.value <= h[best].value) break;
        h[i] = h[best];
        i = best;
    }
    h[i] = last;
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* protection; sequence is 1‑indexed */

    /* 2^n is an upper bound for a(n) – it will be used to decide which factorials matter */
    seq_t limit = pow2(n);                /* 2^n */

    /* Pre‑compute factorial bases ≤ limit. 1! = 1 is always included. */
    const size_t MAX_BASES = 4096;
    seq_t bases[MAX_BASES];
    size_t base_cnt = 0;
    seq_t fac = 1;                        /* 1! */
    bases[base_cnt++] = fac;

    for (idx_t k = 2; k <= n; ++k) {      /* k no larger than n – grows fast enough */
        fac = fac * (seq_t)k;
        if (fac > limit) break;
        if (base_cnt >= MAX_BASES) break;
        bases[base_cnt++] = fac;
    }

    /* Build a min‑heap containing the first power from each base. */
    Node heap[MAX_BASES];
    size_t heap_sz = 0;
    for (size_t i = 0; i < base_cnt; ++i) {
        heap_push(heap, &heap_sz, (Node){bases[i], (unsigned int)i});
    }

    seq_t last = 0;
    size_t count = 0;

    while (1) {
        Node cur = heap_pop(heap, &heap_sz);
        seq_t val = cur.value;
        unsigned int idx = cur.idx;

        if (val != last) {                /* skip duplicates (only 1! causes this) */
            ++count;
            if (count == n) return val;
            last = val;
        }

        /* If the base is 1, further powers would repeat 1, so skip it. */
        if (bases[idx] != 1) {
            seq_t next = val * bases[idx];
            heap_push(heap, &heap_sz, (Node){next, idx});
        }
    }
}
