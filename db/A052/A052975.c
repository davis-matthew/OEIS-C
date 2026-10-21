/*
 * A052975  Expansion of (1-2*x)*(1-x)/(1-5*x+6*x^2-x^3).
 * https://oeis.org/A052975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + a(n-3), a(0)=1, a(1)=2, a(2)=6 (for
 *            n < 3 these are the base cases). The recurrence is computed
 *            incrementally and cached so that any call order is handled
 *            efficiently
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for already computed values */
    static seq_t *cache = NULL;      /* array of computed sequence terms   */
    static size_t cache_len = 0;     /* current number of cached terms      */

    /* ensure base cases are in the cache */
    if (cache_len == 0) {
        cache_len = 3;               /* we will store a(0), a(1), a(2)      */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed (unlikely)       */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
    }

    /* if the requested index is already cached, return it */
    if (n < cache_len)
        return cache[(size_t)n];

    /* grow the cache to accommodate indices up to n */
    cache_len = (size_t)n + 1; /* new required length */
    seq_t *tmp = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0; /* allocation failed */
    cache = tmp;

    /* compute missing terms using the linear recurrence */
    for (size_t i = 3; i <= (size_t)n; ++i) {
        cache[i] = (5 * cache[i-1]) - (6 * cache[i-2]) + cache[i-3];
    }

    return cache[(size_t)n];
}
