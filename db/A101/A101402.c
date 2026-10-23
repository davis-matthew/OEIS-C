/*
 * A101402  a(0)=0, a(1)=1; for n>=2, let k = smallest power of 2 that is >=
 *          n, then a(n) = a(k/2) + a(n-1-k/2).
 * https://oeis.org/A101402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=1; for n≥2 let k be the smallest power of 2 ≥ n,
 *            then a(n)=a(k/2)+a(n-1-k/2)
 * Range:     u8 n <= 719, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;    /* array holding computed values */
static idx_t cache_size = 0;   /* allocated size of cache */
static idx_t max_computed = 1; /* highest index that has been computed */

/* Compute a(n) according to the recurrence above */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Ensure the cache can hold indices up to n */
    if (cache == NULL)
    {
        cache_size = n + 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) abort();                /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        max_computed = 1;
    }
    else if (cache_size <= n)
    {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp) abort();
        cache = tmp;
        cache_size = n + 1;
    }

    /* Compute any missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        /* find smallest power of 2 >= i */
        idx_t k = 1;
        while (k < i) k <<= 1;
        idx_t half = k >> 1;
        idx_t idx2 = i - 1 - half;
        cache[i] = cache[half] + cache[idx2];
    }
    max_computed = n;
    return cache[n];
}
