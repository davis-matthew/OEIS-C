/*
 * A158927  a(n) = -3a(n-1) - 3a(n-2) - 2a(n-3), n > 3.
 * https://oeis.org/A158927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -3a(n-1) - 3a(n-2) - 2a(n-3) with a(0) = a(1) = a(2) = 2,
 *            a(3) = -7
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* memoized values */
    static size_t cached_len = 0;      /* number of elements stored (last index + 1) */

    /* initialise cache with the first four terms once */
    if (cached_len == 0) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)-7;
        cached_len = 4;
    }

    /* return already known term */
    if ((size_t)n < cached_len)
        return cache[(size_t)n];

    /* grow cache to contain the requested term */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (size_t i = cached_len; i <= (size_t)n; ++i) {
        seq_t sum = (seq_t)3 * cache[i - 1] + (seq_t)3 * cache[i - 2] + (seq_t)2 * cache[i - 3];
        cache[i] = -sum;
    }

    cached_len = new_size;
    return cache[(size_t)n];
}
