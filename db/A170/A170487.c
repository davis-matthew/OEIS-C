/*
 * A170487  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1. For n>=1, a(n)=44*∑_{k=1..44}a(n-k) – 990*a(n-45) + p_n,
 *            where p_0=1, p_1..p_44=2, p_45=1, and p_n=0 for n>45
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of filled entries in cache */
static size_t cache_cap = 0;   /* allocated capacity of cache */

/* Ensure that the cache has at least `min_cap` entries allocated. */
static void grow_cache(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    if (new_cap < min_cap)
        new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term of A170487. */
seq_t a(idx_t n)
{
    /* Convert n to size_t for array indexing
       (n is guaranteed to be small, e.g. <= 10000 in tests). */
    size_t idx = (size_t)n;

    /* Extend the cache if necessary. */
    if (cache_len <= idx) {
        grow_cache(idx + 1);          /* need indices 0..n inclusive */
        /* Compute missing values. */
        for (size_t i = cache_len; i <= idx; ++i) {
            seq_t sum = 0;

            /* Coefficient from the numerator (p_i). */
            if (i == 0)
                sum = 1;
            else if (i <= 44)
                sum = 2;
            else if (i == 45)
                sum = 1;
            /* Add 44 * a(i-k) for k = 1 .. min(44, i). */
            size_t maxk = 44;
            if (i < maxk)
                maxk = i;
            for (size_t k = 1; k <= maxk; ++k)
                sum += (seq_t)44 * cache[i - k];
            /* Subtract 990 * a(i-45) if i >= 45. */
            if (i >= 45)
                sum -= (seq_t)990 * cache[i - 45];

            cache[i] = sum;
        }
        cache_len = idx + 1;
    }
    return cache[idx];
}
