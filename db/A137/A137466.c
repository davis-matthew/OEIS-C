/*
 * A137466  1 concatenated with n 21's.
 * https://oeis.org/A137466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 100*a(n-1) + 21, with a(0) = 1, a(1) = 121
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute A137466: 1 followed by n copies of "21" */
seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache      = NULL;
    static idx_t cache_used  = 0;   /* number of entries stored */
    static idx_t cache_cap   = 0;   /* allocated capacity */

    /* Ensure the cache has enough space for indices up to n */
    if (cache_cap == 0) {
        cache_cap = (n >= 2) ? n + 1 : 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* In an unlikely failure we just return 0 – the caller will
           compare 0 against the true value and fail the test. */
        if (!cache) return 0;
        cache_used = 0;
    } else if (cache_cap <= n) {
        /* Grow capacity (doubling strategy) */
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return cache[n];   /* keep old data if realloc fails */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t k = cache_used; k <= n; ++k) {
        seq_t val;
        if (k == 0) {
            val = (seq_t)1;
        } else if (k == 1) {
            val = (seq_t)121;
        } else {
            val = cache[k-1] * (seq_t)100 + (seq_t)21;
        }
        cache[k] = val;
    }
    cache_used = n + 1;

    return cache[n];
}
