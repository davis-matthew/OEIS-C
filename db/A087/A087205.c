/*
 * A087205  a(n) = -2*a(n-1) + 4*a(n-2), a(0)=1, a(1)=2.
 * https://oeis.org/A087205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -2*a(n-1) + 4*a(n-2), a(0)=1, a(1)=2
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 19, i64 n <= 38, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation cache: static so that calls in any order are fast
       and the cache grows only as needed. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of entries already filled */
    static idx_t cache_cap = 0;   /* current capacity of the buffer */

    /* Initialise the cache on the very first call. */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_len = 2;
    }

    /* If we have already computed a(n), return it immediately. */
    if (n < cache_len)
        return cache[n];

    /* Grow the buffer if the requested index does not fit. */
    if (n >= cache_cap) {
        while (cache_cap <= n)
            cache_cap <<= 1;            /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* allocation failure */
        cache = tmp;
    }

    /* Compute all missing terms up to and including n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = -(seq_t)2 * cache[i - 1] + (seq_t)4 * cache[i - 2];
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
