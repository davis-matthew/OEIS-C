/*
 * A015006  q-factorial numbers for q=7.
 * https://oeis.org/A015006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} (7^k-1)/(7-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; a(n) = (7^n - 1)*a(n-1)/6 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static caches for computed values and the current power of 7 */
    static seq_t *cache   = NULL;   /* a[0..cache_len-1] */
    static size_t cache_cap = 0;    /* allocated size of cache */
    static size_t cache_len = 0;    /* number of values stored */

    static seq_t pow7 = 0;          /* 7^current_index, updated each step */
    static bool pow_initialized = false;

    /* On first use initialize a[0] = 1 and set pow7 = 1 (=7^0) */
    if (!pow_initialized)
    {
        pow_initialized = true;
        if (cache_cap == 0)
        {
            cache_cap = 16;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[0] = (seq_t)1;
        cache_len = 1;
        pow7 = (seq_t)1; /* 7^0 */
    }

    /* If requested index is already cached, return it */
    if (n < (idx_t)cache_len)
        return cache[n];

    /* Extend the cache up to index n */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute successive values using the recurrence */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i)
    {
        pow7 = pow7 * (seq_t)7;                  /* pow7 = 7^i */
        seq_t numerator = pow7 - (seq_t)1;       /* 7^i - 1   */
        seq_t prod = numerator * cache[i-1];     /* (7^i-1)*a(i-1) */
        seq_t val = prod / (seq_t)6;             /* divide by 6 */
        cache[i] = val;
    }

    cache_len = (size_t)(n + 1);
    return cache[n];
}
