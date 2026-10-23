/*
 * A136433  a(n+2) = a(n+1)*(n mod 3 + 1) + (n mod 2 + 1), a(1) = 11.
 * https://oeis.org/A136433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=11, a(2)=12; for n>=3, a(n) = a(n-1) * (((n-2) mod 3) + 1)
 *            + (((n-2) mod 2) + 1)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A136433 */
#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;     /* number of values stored */
static size_t cache_cap = 0;     /* capacity of cache */

/* Ensure cache can hold values up to index n (1‑based) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;
    size_t needed = (size_t)n;
    if (cache_cap < needed)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Main function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset 1, undefined for n=0 */

    /* Initialise cache with first two terms if necessary */
    if (cache_len == 0)
    {
        ensure_cache(2);
        cache[0] = (seq_t)11;   /* a(1) */
        cache[1] = (seq_t)12;   /* a(2) */
        cache_len = 2;
    }

    /* Compute missing terms up to n */
    if ((size_t)n > cache_len)
    {
        for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i)
        {
            /* i is current index, compute a(i) */
            idx_t k = i - 2;                /* k = n-2 in the formula */
            seq_t multiplier = (seq_t)((k % 3) + 1);
            seq_t addend     = (seq_t)((k % 2) + 1);
            cache[i - 1] = cache[i - 2] * multiplier + addend;
            ++cache_len;
        }
    }

    return cache[n - 1];
}
