/*
 * A233637  Number of (n+1) X (1+1) 0..2 arrays with every 2 X 2 subblock
 *          having the sum of the squares of the edge differences equal to 2.
 * https://oeis.org/A233637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+6*a(n-2); a(1)=24, a(2)=90
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i+1) for i>=0 */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with first two terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* fallback on allocation failure */
        cache[0] = (seq_t)24;   /* a(1) */
        cache[1] = (seq_t)90;   /* a(2) */
        cache_len = 2;
    }

    /* if the requested term is already stored, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* extend the cache to include up to n */
    /* reallocate enough space first */
    seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (new_cache == NULL) return (seq_t)0;  /* fallback */
    cache = new_cache;

    /* compute new terms using recurrence */
    for (idx_t i = cache_len; i < n; ++i) {
        /* recurrence: a(i+1) = 2*a(i)+6*a(i-1) */
        cache[i] = (seq_t)2 * cache[i - 1] + (seq_t)6 * cache[i - 2];
    }

    cache_len = n;
    return cache[n - 1];
}
