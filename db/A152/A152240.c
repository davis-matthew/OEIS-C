/*
 * A152240  a(n)=5*a(n-1)+7*a(n-2), n>1 ; a(0)=1, a(1)=7 .
 * https://oeis.org/A152240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 5*a(n-1)+7*a(n-2), a(0)=1, a(1)=7
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;

static void ensure_capacity(idx_t n)
{
    if ((idx_t)cache_cap <= n)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *new_cache = seq_alloc_array(new_cap);
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        if (cache)
            seq_free_array(cache);
        cache      = new_cache;
        cache_cap  = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)7;

    ensure_capacity(n);
    while (cache_len <= (size_t)n)
    {
        size_t i = cache_len;
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)7;
        else
            cache[i] = 5 * cache[i - 1] + 7 * cache[i - 2];
        ++cache_len;
    }
    return cache[(size_t)n];
}
