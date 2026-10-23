/*
 * A049674  a(n) = (F(3*n) - 2*F(n))/6, where F=A000045 (the Fibonacci
 *          sequence).
 * https://oeis.org/A049674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (F(3*n) - 2*F(n))/6
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=0, a(2)=1, a(3)=5;
   For n>=4: a(n) = 5*a(n-1) - 2*a(n-2) - 5*a(n-3) - a(n-4) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with first four values */
    if (cache_size == 0) {
        cache_size = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache)
            return 0; /* allocation failed */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 1;
        cache[3] = 5;
    }

    if (n < cache_size)
        return cache[n];

    /* extend cache up to n */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    if (!new_cache)
        return 0; /* allocation failed */
    cache = new_cache;

    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = 5 * cache[i-1];
        val -= 2 * cache[i-2];
        val -= 5 * cache[i-3];
        val -=  cache[i-4];
        cache[i] = val;
    }

    cache_size = new_size;
    return cache[n];
}
