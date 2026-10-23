/*
 * A147748  Row sums of Riordan array ((1-3x+x^2)/(1-4x+3x^2),
 *          x(1-2x)/(1-4x+3x^2)).
 * https://oeis.org/A147748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2) a(0) = 1, a(1) = 2, a(2) = 6 (valid
 *            for all n >= 0)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Ensure the base values are stored */
    if (cache_len < 3) {
        size_t needed = 3;
        if (cache_len == 0) {
            cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache_len = 3;
    }

    /* If the requested index is already computed, return it */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Grow the cache to accommodate the new index */
    size_t new_len = (size_t)n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    /* Compute missing terms using the recurrence */
    for (size_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)5 * cache[i - 1] - (seq_t)5 * cache[i - 2];
    }
    cache_len = new_len;

    return cache[n];
}
