/*
 * A121621  Real part of (2 + 3i)^n.
 * https://oeis.org/A121621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*a(n-1) - 13*a(n-2), a(0) = 1, a(1) = 2
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 17, i64 n <= 34, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise first two values */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
    }

    /* return already computed value */
    if (n < cache_len)
        return cache[(size_t)n];

    /* grow the cache to hold up to index n */
    seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) return 0;              /* allocation failed */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (4 * cache[i - 1]) - (13 * cache[i - 2]);
    }
    cache_len = n + 1;

    return cache[(size_t)n];
}
