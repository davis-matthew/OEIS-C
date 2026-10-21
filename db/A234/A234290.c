/*
 * A234290  Duplicate of A095839.
 * https://oeis.org/A234290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 13, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1,
   a(n) = (4*n-9)*a(n-1) + 6*(n-1)*(2*n-3)*a(n-2)   (n >= 2) */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of allocated elements */

    /* initialise cache with base values on first call */
    if (cache_size == 0) {
        cache = (seq_t *)calloc(2, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;           /* a(0) */
        cache[1] = 1;           /* a(1) */
        cache_size = 2;
    }

    /* if the requested index exceeds the cache, grow and compute */
    size_t req = (size_t)n + 1;   /* elements needed up to n */
    if (req > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, req * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* initialise new slots to zero */
        for (size_t i = cache_size; i < req; ++i) cache[i] = 0;

        /* compute the missing terms */
        for (size_t i = cache_size; i < req; ++i) {
            seq_t coeff1 = (seq_t)(4LL * (int64_t)i - 9LL);
            seq_t coeff2 = (seq_t)(6LL * (int64_t)(i-1) *
                                   (2LL * (int64_t)i - 3LL));
            cache[i] = coeff1 * cache[i-1] + coeff2 * cache[i-2];
        }
        cache_size = req;
    }

    return cache[(size_t)n];
}
