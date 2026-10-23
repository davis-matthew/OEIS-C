/*
 * A025927  Expansion of 1/((1-2*x)*(1-3*x)*(1-4*x)*(1-10*x)).
 * https://oeis.org/A025927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*a(n-1) - 116*a(n-2) + 284*a(n-3) - 240*a(n-4) with
 *            a(0)=1, a(1)=19, a(2)=245, a(3)=2735
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* a(0) ... a(k) */
static idx_t  cache_size = 0;    /* number of elements stored */
static size_t cache_capacity = 0;/* allocated array size */

/* Ensure the cache can hold at least idx entries */
static void ensure_capacity(idx_t idx)
{
    if ((size_t)idx < cache_capacity)
        return;

    size_t new_cap = cache_capacity ? cache_capacity << 1 : 4;
    while (new_cap <= (size_t)idx)
        new_cap <<= 1;

    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf) abort();
    cache        = new_buf;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise cache with base values on first call */
    if (!cache)
    {
        cache_capacity = 4;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) abort();

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)19;
        cache[2] = (seq_t)245;
        cache[3] = (seq_t)2735;
        cache_size = 4;
    }

    if (n < cache_size)
        return cache[(size_t)n];

    for (idx_t i = cache_size; i <= n; ++i)
    {
        if ((size_t)i >= cache_capacity)
            ensure_capacity(i);

        seq_t val = cache[(size_t)(i - 1)] * (seq_t)19
                  - cache[(size_t)(i - 2)] * (seq_t)116
                  + cache[(size_t)(i - 3)] * (seq_t)284
                  - cache[(size_t)(i - 4)] * (seq_t)240;
        cache[(size_t)i] = val;
    }

    cache_size = n + 1;
    return cache[(size_t)n];
}
