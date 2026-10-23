/*
 * A159480  Numerator of Hermite(n, 5/12).
 * https://oeis.org/A159480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=5, a(n)=5*a(n-1)-72*(n-1)*a(n-2) for n≥2
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 14, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    if (cache == NULL) {
        cache_cap = 4;                     /* initial capacity */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        cache[0] = 1;                      /* a(0) */
        cache[1] = 5;                      /* a(1) */
        cache_len = 2;                     /* next free index */
    }

    /* grow the cache if needed */
    if (n >= cache_cap) {
        while (cache_cap <= n) cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* compute missing values up to n */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            /* a(i) = 5*a(i-1) - 72*(i-1)*a(i-2) */
            seq_t prev      = cache[i - 1];
            seq_t prevprev  = cache[i - 2];
            seq_t factor    = ((seq_t)(i - 1)) * 72;
            cache[i] = 5 * prev - factor * prevprev;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
