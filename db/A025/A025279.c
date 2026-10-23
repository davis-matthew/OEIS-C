/*
 * A025279  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ...+ a(n-1)*a(1) for n >= 5,
 *          starting 1,0,-1,1.
 * https://oeis.org/A025279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ... + a(n-1)*a(1) with
 *            a(1)=1, a(2)=0, a(3)=-1, a(4)=1
 * Range:     i8 n <= 11, i16 n <= 18, i32 n <= 30, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* Static cache for memoisation of already computed values. */
    static seq_t   *cache      = NULL;
    static idx_t    cache_size = 0; /* allocated slot count */
    static idx_t    max_computed = 0; /* largest n known */

    /* Initialise for the first call: store the first four values. */
    if (cache == NULL) {
        cache_size = 5;          /* indices 0..4, index 0 unused */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;    /* allocation failure */
        cache[0] = 0;            /* unused placeholder */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)(-1);
        cache[4] = (seq_t)1;
        max_computed = 4;
    }

    /* If the requested n is already computed, return it. */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Grow the cache if the desired index exceeds its current size. */
    if ((idx_t)(n + 1) > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure */
        cache      = new_cache;
        cache_size = n + 1;
    }

    /* Compute missing values up to n using the defining convolution. */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        /* Only indices k>=5 need the convolution; 1..4 are already set. */
        if (k < 5) {
            continue;
        }
        seq_t sum = 0;
        for (idx_t i = 1; i < k; ++i) {
            sum += cache[i] * cache[k - i];
        }
        cache[k] = sum;
    }

    max_computed = n;
    return cache[n];
}
