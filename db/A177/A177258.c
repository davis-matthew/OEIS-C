/*
 * A177258  Number of derangements of {1,2,...,n} having no adjacent
 *          transpositions.
 * https://oeis.org/A177258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) for OEIS A177258
 *
 *  Formula (recurrence):
 *      a(0) = 1,  a(1) = 0,  a(2) = 0,  a(3) = 2
 *      a(n) = (n-1)*a(n-1) + (n-1)*a(n-2) + (n-1)*a(n-3) + a(n-4)
 *          for all n ≥ 4
 *
 *  This implementation memoises computed values in a static array.
 *  It works for any seq_t that supports +, -, *, and the small‑int cast
 *  (e.g. an arbitrary‑precision integer class or any unsigned integer type).
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache with known values on the first call */
    if (cache == NULL) {
        cache_size = 4;                                   /* indices 0..3 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;                      /* OOM guard */
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)0;   /* a(1) */
        cache[2] = (seq_t)0;   /* a(2) */
        cache[3] = (seq_t)2;   /* a(3) */
    }

    /* If the value is already cached, return it */
    if (n < cache_size) return cache[n];

    /* Grow the cache to hold index n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;                      /* OOM guard */
    size_t prev_size = cache_size;                  /* old number of terms */
    cache = tmp;
    cache_size = (idx_t)new_size;

    /* Compute missing terms using the recurrence */
    for (idx_t i = prev_size; i <= n; ++i) {
        seq_t factor = (seq_t)(i - 1);
        seq_t val = factor * cache[i - 1];
        val += factor * cache[i - 2];
        val += factor * cache[i - 3];
        val += cache[i - 4];
        cache[i] = val;
    }

    return cache[n];
}
