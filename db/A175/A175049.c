/*
 * A175049  y-values in the solution to x^2-59*y^2=1.
 * https://oeis.org/A175049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1060*a(n-1)-a(n-2) with a(1)=0, a(2)=69
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_n = 0;

    /* initialize cache with the first two terms */
    if (!cache) {
        cache_n = 2;
        cache = (seq_t *)malloc(cache_n * sizeof(seq_t));
        if (!cache) exit(1);        /* unlikely to happen on test machines */
        cache[0] = (seq_t)0;       /* a(1) */
        cache[1] = (seq_t)69;      /* a(2) */
    }

    /* if already computed, return directly */
    if (n <= cache_n)
        return cache[n - 1];

    /* extend the cache and compute new values */
    cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!cache) exit(1);           /* unlikely to happen */
    for (idx_t k = cache_n + 1; k <= n; ++k) {
        /* a(k) = 1060 * a(k-1) - a(k-2) */
        seq_t val = (seq_t)1060 * cache[k - 2] - cache[k - 3];
        cache[k - 1] = val;
    }
    cache_n = n;
    return cache[n - 1];
}
