/*
 * A160251  Numerator of Hermite(n, 8/29).
 * https://oeis.org/A160251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 16*a(n-1) - 1682*n*a(n-2); a(0) = 1, a(1) = 16 (derived
 *            from 29^n*Hermite(n,8/29)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with the first two terms */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t*)realloc(cache, cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)16;
    }

    /* if requested value is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* grow cache up to n+1 and compute missing terms */
    const idx_t new_len = n + 1;
    cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    for (idx_t i = cache_len; i < new_len; ++i) {
        /* a(i) = 16*a(i-1) - 1682*(i-1)*a(i-2) */
        seq_t term = (seq_t)16 * cache[i-1]
                   - (seq_t)1682 * (seq_t)(i-1) * cache[i-2];
        cache[i] = term;
    }
    cache_len = new_len;
    return cache[n];
}
