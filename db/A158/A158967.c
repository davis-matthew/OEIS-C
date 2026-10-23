/*
 * A158967  Numerator of Hermite(n, 4/5).
 * https://oeis.org/A158967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   from OEIS A158967 a(0) = 1 a(1) = 8 a(n) = 8*a(n-1) -
 *            50*(n-1)*a(n-2) for n>=2 (Recurrence: a(n) - 8*a(n-1) +
 *            50*(n-1)*a(n-2) = 0)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 15, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of values stored (e.g. up to cache_len-1) */

    /* if we already have the value, return it immediately */
    if (n < cache_len)
        return cache[n];

    /* ensure we have space for at least n+1 values */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise base values if this is the first call */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        if (cache_len < 2)
            cache[1] = (seq_t)8;
        cache_len = 2;
    }
    /* if cache_len is 1 (shouldn't happen, but be safe) */
    else if (cache_len == 1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache_len = 2;
    }

    /* compute values up to n using the recurrence */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        /* a(i) = 8*a(i-1) - 50*(i-1)*a(i-2)  */
        seq_t term = (seq_t)8 * cache[i - 1]
                     - (seq_t)50 * (seq_t)(i - 1) * cache[i - 2];
        cache[i] = term;
    }
    cache_len = (size_t)n + 1;      /* now we have values 0..n */

    return cache[n];
}
