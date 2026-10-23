/*
 * A110311  Expansion of 1/((1+x+x^2)*(1+5*x+x^2)).
 * https://oeis.org/A110311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -6*a(n-1) - 7*a(n-2) - 6*a(n-3) - a(n-4) for n >= 4 *
 *            Initial values: * a(0) = 1 * a(1) = -6 * a(2) = 29 * a(3) = -138
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 13, i64 n <= 27, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache with first four terms if needed */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;  /* fallback on allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-6;
        cache[2] = (seq_t)29;
        cache[3] = (seq_t)-138;
    }

    /* Return immediately if we already computed enough terms */
    if (n < cache_len)
        return cache[n];

    /* Extend the cache up to the requested index */
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) return 0;  /* fallback on allocation failure */
    cache = new_cache;

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term1 = ((seq_t)6) * cache[i - 1];
        seq_t term2 = ((seq_t)7) * cache[i - 2];
        seq_t term3 = ((seq_t)6) * cache[i - 3];
        seq_t term4 = cache[i - 4];
        seq_t sum    = term1 + term2 + term3 + term4;
        cache[i]     = -sum;
    }
    cache_len = new_len;
    return cache[n];
}
