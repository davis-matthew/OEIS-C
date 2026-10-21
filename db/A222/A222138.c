/*
 * A222138  Number of nX2 0..4 arrays with no entry increasing mod 5 by 4
 *          rightwards or downwards, starting with upper left zero.
 * https://oeis.org/A222138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*13^(n-1) for n >= 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses a static cache to enable efficient O(n) computation
   even when calls are in arbitrary order. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[0] corresponds to a(1) */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;

    /* initialize cache for n == 1 */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory: return 0 */
        cache[0] = (seq_t)4;
        cache_len = 1;
    }

    /* grow cache up to size n */
    if (n > cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;      /* out of memory: return 0 */
        cache = new_cache;

        /* build remaining terms */
        for (idx_t i = cache_len; i < n; ++i) {
            /* a(i+1) = a(i) * 13 */
            cache[i] = cache[i-1] * (seq_t)13;
        }
        cache_len = n;
    }

    return cache[n-1];
}
