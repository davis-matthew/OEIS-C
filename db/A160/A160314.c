/*
 * A160314  Numerator of Hermite(n, 16/31).
 * https://oeis.org/A160314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 32*a(n-1) - 1922*(n-1)*a(n-2) a(0)=1, a(1)=32
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cache of previously computed terms   */
    static idx_t cache_len = 0;    /* number of terms stored (indices 0..len-1) */

    /* Initialisation: store the two seed values */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)32;
    }

    /* Extend cache up to the requested index */
    if (n >= cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!new_cache) {
            /* Allocation failed – this should not happen under normal limits */
            return (seq_t)0;
        }
        cache = new_cache;

        /* Compute missing terms using the recurrence:
           a(i) = 32*a(i-1) - 1922*(i-1)*a(i-2)   for i >= 2  */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prod = (seq_t)1922 * (seq_t)(i - 1) * cache[i - 2];
            cache[i] = (seq_t)32 * cache[i - 1] - prod;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
