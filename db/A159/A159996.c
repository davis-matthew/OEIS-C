/*
 * A159996  Numerator of Hermite(n, 17/24).
 * https://oeis.org/A159996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 12^n * Hermite(n, 17/24) Recurrence: a(0) = 1, a(1) = 17
 *            a(n) = 17*a(n-1) - 288*(n-1)*a(n-2) for n >= 2
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n + 1 > cache_size) {
        /* Allocate or grow the cache */
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* Initialise initial terms if the cache was empty or incomplete */
        if (cache_size == 0) {
            cache[0] = 1;                 /* a(0) */
            if (n >= 1) cache[1] = 17;     /* a(1) */
        } else if (cache_size == 1) {
            /* cache[0] already set */
            cache[1] = 17;                /* a(1) */
        }

        /* Determine from which index to start computing */
        idx_t start = cache_size;
        if (cache_size == 0 || cache_size == 1) start = 2;

        /* Compute terms from start up to n */
        for (idx_t k = start; k <= n; ++k) {
            seq_t term1 = 17 * cache[k - 1];
            seq_t term2 = cache[k - 2] * 288;
            term2 *= (seq_t)(k - 1);
            cache[k] = term1 - term2;
        }

        cache_size = n + 1;
    }

    return cache[n];
}
