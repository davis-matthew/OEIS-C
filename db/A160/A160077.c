/*
 * A160077  Numerator of Hermite(n, 19/26).
 * https://oeis.org/A160077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 19*a(n-1) - 338*(n-1)*a(n-2) Initial values: a(0) = 1,
 *            a(1) = 19 This implements the recurrence a(n)
 *            -19*a(n-1)+338*(n-1)*a(n-2)=0
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 22,
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
    static idx_t cache_size = 0;   /* number of entries currently stored */
    static idx_t cache_cap  = 0;   /* allocated capacity */

    /* ensure we have space for indices up to n */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (cache_cap == 0 ? 1 : cache_cap * 2);
        while (new_cap <= n) new_cap <<= 1;          /* double until big enough */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute from the first missing entry up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)19;
        } else {
            /* a(i) = 19*a(i-1) - 338*(i-1)*a(i-2) */
            seq_t term1 = (seq_t)19 * cache[i - 1];
            seq_t term2 = (seq_t)338 * (seq_t)(i - 1) * cache[i - 2];
            cache[i] = term1 - term2;
        }
    }
    cache_size = n + 1;
    return cache[n];
}
