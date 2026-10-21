/*
 * A233628  Number of (n+1) X (1+1) 0..3 arrays with every 2 X 2 subblock
 *          having the sum of the absolute values of the edge differences
 *          equal to 6.
 * https://oeis.org/A233628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 19*a(n-2) - 27*a(n-3) - 55*a(n-4) + 37*a(n-5),
 *            with a(1)=100, a(2)=716, a(3)=4974, a(4)=34996, a(5)=245244
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Computes the n-th term of A233628.  n ≥ 1. */
seq_t a(idx_t n)
{
    /* ---------- memoization state ---------- */
    static seq_t *cache      = NULL;     /* array storing computed terms */
    static size_t cache_cap  = 0;        /* capacity of cache */
    static idx_t cached_n    = 0;        /* maximum n already computed */

    /* ---------- ensure enough space ---------- */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failed – return 0 as sentinel */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ---------- initial values for n = 1 … 5 ---------- */
    static const seq_t init[5] = {
        (seq_t)100,
        (seq_t)716,
        (seq_t)4974,
        (seq_t)34996,
        (seq_t)245244
    };
    if (cached_n < 5) {
        idx_t start = cached_n + 1;
        idx_t end   = n < 5 ? n : 5;
        for (idx_t i = start; i <= end; ++i) {
            cache[i-1] = init[i-1];
        }
    }

    /* ---------- compute terms by recurrence for n >= 6 ---------- */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        if (i <= 5) continue; /* already handled */
        size_t idx = (size_t)(i - 1);            /* zero‑based index in cache */
        seq_t tmp  = cache[idx-1] * (seq_t)5 +
                     cache[idx-2] * (seq_t)19 +
                     cache[idx-5] * (seq_t)37;
        tmp -= cache[idx-3] * (seq_t)27;
        tmp -= cache[idx-4] * (seq_t)55;
        cache[idx] = tmp;
    }

    /* ---------- update state ---------- */
    if (n > cached_n)
        cached_n = n;

    return cache[(size_t)(n - 1)];
}
