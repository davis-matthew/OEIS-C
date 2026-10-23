/*
 * A159616  Expansion of (1-x)/(1-5*x-2*x^2+8*x^3).
 * https://oeis.org/A159616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) + 2*a(n-2) - 8*a(n-3) with a(0)=1, a(1)=4,
 *            a(2)=22
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoization */
static seq_t *cache = NULL;       /* Allocated array of values */
static idx_t cache_len = 0;       /* Number of filled entries */
static size_t cache_cap = 0;      /* Allocated capacity */

/* Ensure the cache has room for indices up to n (inclusive) */
static void ensure_cache_space(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 3;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the cache with the three known initial values */
static void init_cache(void)
{
    if (cache_cap != 0)
        return;
    ensure_cache_space(2);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)4;
    cache[2] = (seq_t)22;
    cache_len = 3;
}

seq_t a(idx_t n)
{
    init_cache();

    /* Return a value that is already calculated */
    if (n < cache_len)
        return cache[n];

    /* Extend the cache up to n (inclusive) */
    ensure_cache_space(n);

    for (idx_t i = cache_len; i <= n; ++i) {
        /* Recurrence: a(i) = 5*a(i-1) + 2*a(i-2) - 8*a(i-3) */
        seq_t val = (seq_t)5 * cache[i - 1]
                  + (seq_t)2 * cache[i - 2]
                  - (seq_t)8 * cache[i - 3];
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
