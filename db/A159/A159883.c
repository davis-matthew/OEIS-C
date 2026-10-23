/*
 * A159883  Numerator of Hermite(n, 14/23).
 * https://oeis.org/A159883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 28*a(n-1) - 1058*(n-1)*a(n-2), with a(0) = 1, a(1) = 28.
 *            This implements the recurrence from OEIS A159883 (scaled Hermite
 *            numbers)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for NULL, size_t */
#include <stdlib.h>   /* for malloc, realloc */

/* Static cache for memoization */
static seq_t *cache = NULL;       /* array of computed terms */
static idx_t  cache_len = 0;      /* number of cached terms */

/* Returns a(n) */
seq_t a(idx_t n)
{
    /* Initialise the cache with the first two terms if needed */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) {
            /* In case of allocation failure, just return 0 */
            return (seq_t)0;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)28;
        cache_len = 2;
    }

    /* For n that is already cached, return immediately */
    if (n < cache_len) {
        return cache[n];
    }

    /* Expand the cache to hold terms up to n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (new_cache == NULL) {
        /* In case of allocation failure, return 0 */
        return (seq_t)0;
    }
    cache = new_cache;

    /* Compute any missing terms using the recurrence */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t term =
            (seq_t)28 * cache[k - 1]                  /* 28*a(k-1) */
            - (seq_t)1058 * (seq_t)(k - 1) * cache[k - 2]; /* 1058*(k-1)*a(k-2) */
        cache[k] = term;
    }

    cache_len = n + 1;
    return cache[n];
}
