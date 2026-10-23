/*
 * A163064  a(n) = ((3+sqrt(5))*(4+sqrt(5))^n + (3-sqrt(5))*(4-sqrt(5))^n)/2.
 * https://oeis.org/A163064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 11*a(n-2), a(0)=3, a(1)=17
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache   = NULL;
    static idx_t cache_size = 0;
    static size_t cache_cap  = 0;
    /* initialize cache on first call */
    if (!cache) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)3;   /* a(0) */
        cache[1] = (seq_t)17;  /* a(1) */
        cache_size = 2;
    }
    /* return already computed value */
    if (n < cache_size)
        return cache[n];

    /* grow cache if needed */
    while (cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute values up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = (seq_t)8 * cache[i - 1] - (seq_t)11 * cache[i - 2];
        cache[i] = val;
    }
    cache_size = n + 1;
    return cache[n];
}
