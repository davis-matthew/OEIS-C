/*
 * A049683  a(n) = (Lucas(6*n) - 2)/16.
 * https://oeis.org/A049683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*a(n-1) - 19*a(n-2) + a(n-3) with a(0)=0, a(1)=1,
 *            a(2)=20
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_sz = 0;           /* number of elements currently cached */

/* Ensure that cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_sz)
        return;

    idx_t new_cap = cache_sz ? cache_sz * 2 : 1;
    if (new_cap <= n)
        new_cap = n + 1;

    /* realloc returns void*; cast for C++ compatibility */
    cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));

    for (idx_t i = cache_sz; i < new_cap; ++i)
    {
        if (i == 0)
            cache[i] = (seq_t)0;
        else if (i == 1)
            cache[i] = (seq_t)1;
        else if (i == 2)
            cache[i] = (seq_t)20;
        else
            cache[i] = ((seq_t)19 * cache[i - 1]) -
                       ((seq_t)19 * cache[i - 2]) + cache[i - 3];
    }

    cache_sz = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
