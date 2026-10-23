/*
 * A163066  a(n) = 12*a(n-1) - 31*a(n-2) for n > 1; a(0) = 2, a(1) = 17.
 * https://oeis.org/A163066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 31*a(n-2) for n > 1; a(0) = 2, a(1) = 17
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cache of computed terms */
    static size_t  cache_len = 0;        /* number of terms stored (may be 0) */

    /* if requested term already in cache, return it */
    if (n < cache_len)
        return cache[n];

    /* determine new cache size needed */
    size_t new_len = (size_t)n + 1;

    /* allocate or grow cache */
    if (cache_len == 0)
        cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

    /* handle base cases */
    if (cache_len == 0) {
        cache[0] = (seq_t)2;
        cache_len = 1;
    }
    if (n >= 1 && cache_len <= 1) {
        cache[1] = (seq_t)17;
        cache_len = 2;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
        cache[i] = (12 * cache[i - 1]) - (31 * cache[i - 2]);

    cache_len = new_len;
    return cache[n];
}
