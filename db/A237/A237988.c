/*
 * A237988  a(n) = 3*a(n-3) + 3*a(n-6) + a(n-9) for n>8, a(0)=0, a(1)=a(2)=1,
 *          a(3)=a(4)=2, a(5)=3, a(6)=7, a(7)=9, a(8)=11.
 * https://oeis.org/A237988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=a(2)=1, a(3)=a(4)=2, a(5)=3, a(6)=7, a(7)=9,
 *            a(8)=11, and for n>8: a(n)=3*a(n-3)+3*a(n-6)+a(n-9)
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 51, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t  *cache      = NULL;
static size_t cache_size  = 0;
static size_t cache_cap   = 0;

/* Grow the cache so that it can hold indices up to 'needed-1'.  */
static void
ensure_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_cache = alloc_seq_array(new_cap);
    for (size_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    free_seq_array(cache);
    cache     = new_cache;
    cache_cap = new_cap;
}

seq_t
a(idx_t n)
{
    /* Use size_t for array indexing.  */
    size_t idx = (size_t)n;

    /* Make sure the cache can hold the required index.  */
    ensure_capacity(idx + 1);

    /* Compute any missing terms up to 'idx'.  */
    for (size_t i = cache_size; i <= idx; ++i)
    {
        if (i == 0)
            cache[0] = 0;
        else if (i == 1 || i == 2)
            cache[i] = 1;
        else if (i == 3 || i == 4)
            cache[i] = 2;
        else if (i == 5)
            cache[i] = 3;
        else if (i == 6)
            cache[i] = 7;
        else if (i == 7)
            cache[i] = 9;
        else if (i == 8)
            cache[i] = 11;
        else
            cache[i] = 3 * cache[i - 3] + 3 * cache[i - 6] + cache[i - 9];
    }

    cache_size = idx + 1;
    return cache[idx];
}
