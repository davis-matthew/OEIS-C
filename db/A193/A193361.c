/*
 * A193361  a(0)=0, a(1)=0; for n>1, a(n) = a(n-1) + (n-3)*a(n-2) + 1.
 * https://oeis.org/A193361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0; for n>1, a(n)=a(n-1)+(n-3)*a(n-2)+1
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 33, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoization across calls */
static seq_t *cache = NULL;       /* dynamically allocated array of a(n) */
static idx_t cache_cap = 0;       /* current capacity of the cache */
static idx_t cache_len = 0;       /* number of elements currently stored */

/* Ensure that terms up to index 'upto' are computed and cached */
static void ensure_cache_upto(idx_t upto)
{
    /* Grow the cache lazily; only called for n >= 2 */
    while (cache_len <= upto)
    {
        /* Initial allocation or grow when full */
        if (cache_len == 0)
        {
            cache_cap = 4;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            cache[0] = 0;   /* a(0) */
            cache[1] = 0;   /* a(1) */
            cache_len = 2;
        }
        else if (cache_len == cache_cap)
        {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache,
                                      cache_cap * sizeof(seq_t));
        }

        /* Compute next term */
        idx_t i = cache_len;          /* index of new term to compute */
        seq_t val;
        if (i == 2)                   /* special case to avoid unsigned wrap */
            val = cache[1] + 1;
        else
            val = cache[i-1] + (seq_t)(i-3) * cache[i-2] + 1;

        cache[cache_len++] = val;
    }
}

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0 || n == 1)
        return 0;

    /* Compute missing terms if necessary */
    ensure_cache_upto(n);

    return cache[n];
}
