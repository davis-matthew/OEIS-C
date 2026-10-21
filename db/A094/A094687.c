/*
 * A094687  Convolution of Fibonacci and Jacobsthal numbers.
 * https://oeis.org/A094687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n‑1) + 2*a(n‑2) – 3*a(n‑3) – 2*a(n‑4) initial values:
 *            a(0)=0, a(1)=0, a(2)=1, a(3)=2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* Ensure cache has enough capacity */
    if (n >= cache_cap)
    {
        /* Grow to at least n+1, doubling strategy */
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* fallback on allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialize first four terms if needed */
    if (cache_len == 0)
    {
        cache[0] = 0;          /* a(0) */
        cache[1] = 0;          /* a(1) */
        cache[2] = 1;          /* a(2) */
        cache[3] = 2;          /* a(3) */
        cache_len = 4;
    }

    /* Compute up to requested n */
    while (cache_len <= n)
    {
        idx_t i = cache_len;    /* current index to compute */
        seq_t term = cache[i-1] + cache[i-1];          /* 2*a(i-1) */
        term += cache[i-2] + cache[i-2];              /* +2*a(i-2) */
        term -= cache[i-3] * (seq_t)3;                /* –3*a(i-3) */
        term -= cache[i-4] * (seq_t)2;                /* –2*a(i-4) */
        cache[i] = term;
        cache_len = i + 1;
    }

    return cache[n];
}
