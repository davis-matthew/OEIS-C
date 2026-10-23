/*
 * A165155  a(n) = 100*a(n-1) + 11^(n-1) for n>0, a(0)=0.
 * https://oeis.org/A165155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*a(n-1) + 11^(n-1), a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Store previously computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    /* Power of 11 for the next term. Initially 11^0. */
    static seq_t power = (seq_t)1;

    /* Ensure the cache is initialized for n = 0. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* Out of memory: return 0. */
        cache[0] = (seq_t)0;
        cache_size = 1;
    }

    /* If the requested index is already computed, return it. */
    if ((size_t)n < cache_size)
        return cache[n];

    /* Extend the cache to hold term n. */
    size_t needed = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (tmp == NULL) return (seq_t)0;   /* Out of memory: return 0. */
    cache = tmp;

    /* Compute terms from the last known index up to n. */
    size_t i;
    for (i = cache_size; i <= n; ++i) {
        cache[i] = (cache[i - 1] * (seq_t)100) + power;
        power = power * (seq_t)11;
    }

    cache_size = needed;
    return cache[n];
}
