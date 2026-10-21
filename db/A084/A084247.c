/*
 * A084247  a(n) = -a(n-1) + 2*a(n-2), a(0)=1, a(1)=2.
 * https://oeis.org/A084247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = 1, a(1) = 2, a(n) = -a(n-1) + 2*a(n-2) for n ≥ 2
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow arbitrary call order and memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms stored */
    static idx_t cache_cap  = 0;   /* current capacity of cache */

    /* Initialise cache with the first two terms */
    if (!cache) {
        cache_cap  = 4;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed, unlikely here */
        cache[0] = 1;
        cache[1] = 2;
        cache_size = 2;
    }

    /* Return directly if value is already computed */
    if (n < cache_size)
        return cache[n];

    /* Extend cache up to n using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i >= cache_cap) {          /* grow cache when needed */
            cache_cap = cache_cap * 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache) return 0;      /* allocation failed, unlikely here */
        }
        seq_t val = -cache[i-1] + (seq_t)2 * cache[i-2];
        cache[i] = val;
        cache_size++;
    }

    return cache[n];
}
