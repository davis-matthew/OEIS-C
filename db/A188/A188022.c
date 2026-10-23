/*
 * A188022  Expansion of x*(1+x) / (1-3*x^2-x^3).
 * https://oeis.org/A188022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) + a(n-3) (a(0)=0, a(1)=1, a(2)=1)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 71, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache       = NULL;
    static size_t cache_cap  = 0;   /* capacity (number of elements) */
    static size_t computed_to = 0;  /* largest index that has been computed */

    /* Lazy initialization of the first few terms */
    if (cache == NULL) {
        cache_cap      = 4;         /* indices 0..3 */
        cache          = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out‑of‑memory */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        computed_to = 3;
    }

    /* If n is beyond current capacity, grow the cache */
    size_t n_us = (size_t)n;  /* safe as n is small (≤ 10 000 for tests) */
    if (n_us >= cache_cap) {
        size_t new_cap = n_us + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out‑of‑memory */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    if (n_us > computed_to) {
        for (idx_t i = (idx_t)(computed_to + 1); i <= n; ++i) {
            /* a(i) = 3*a(i-2) + a(i-3) */
            cache[i] = (seq_t)3 * cache[i - 2] + cache[i - 3];
        }
        computed_to = n_us;
    }

    return cache[n];
}
