/*
 * A107403  Expansion of e.g.f. 1/(1 - 3*sinh(x)).
 * https://oeis.org/A107403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 3 * Σ_{k=0..⌊(n‑1)/2⌋} binomial(n, 2k+1) *
 *            a(n‑2k‑1) (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   binom_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the recurrence using memoisation. */

#include <stddef.h>           /* for size_t */
#include <stdlib.h>          /* for realloc */
#include "../utils/binom_small.h"   /* binom_small(idx_t N, idx_t K) */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;     /* allocated capacity of cache */
    static idx_t  size  = 0;     /* number of valid entries in cache */

    /* Ensure the cache is large enough to hold a[n] */
    if (size <= n) {
        size_t new_cap = (cap == 0) ? 1 : cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing values up to n */
    for (idx_t i = size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
            continue;
        }
        seq_t sum = 0;
        for (idx_t k = 0; ; ++k) {
            idx_t odd = 2 * k + 1;
            if (odd > i)
                break;
            seq_t bin   = binom_small(i, odd);
            seq_t term  = bin * cache[i - odd];
            sum += term;
        }
        cache[i] = 3 * sum;
    }
    size = n + 1;
    return cache[n];
}
