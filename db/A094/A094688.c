/*
 * A094688  Convolution of Fibonacci(n) and 3^n.
 * https://oeis.org/A094688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 3^(n-1), a(0)=0, a(1)=1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow O(1) lookup on already computed indices.
       The cache grows as needed. */
    static seq_t *cache   = NULL;
    static idx_t  cache_len = 0;
    static idx_t  cache_cap = 0;
    static seq_t  pow3;          /* holds 3^(cache_len-1) for the next element */

    /* Initialise cache on first entry. */
    if (cache == NULL)
    {
        cache_cap = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
        pow3 = (seq_t)3;  /* 3^(1) for computing a(2) */
    }

    /* Return already computed value if within cache. */
    if (n < cache_len)
        return cache[n];

    /* Grow cache if necessary. */
    while (cache_len <= n)
    {
        idx_t i = cache_len;   /* next index to compute */

        /* Compute a(i) using recurrence */
        seq_t val = cache[i-1] + cache[i-2] + pow3;

        /* Append to cache */
        if (cache_len == cache_cap)
        {
            idx_t new_cap = cache_cap * 2;
            if (new_cap < 8) new_cap = 8;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            cache_cap = new_cap;
        }
        cache[cache_len] = val;
        cache_len++;

        /* Update power of 3 for the next iteration */
        pow3 *= (seq_t)3;
    }

    return cache[n];
}
