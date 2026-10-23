/*
 * A032513  Sum of the integer part of 5th roots of positive integers less
 *          than or equal to n.
 * https://oeis.org/A032513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n} floor(k^(1/5))
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   root5_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. a(n) = a(n-1) + floor(n^(1/5)),  a(0)=0 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/root5_uint64.h"

/* Cached values of the sequence */
static seq_t *a_cache = NULL;          /* array holding a(0)..a(k) */
static size_t cache_cap = 0;           /* capacity in number of elements */
static size_t cache_len = 0;           /* actual number of filled elements (≥1, except maybe before first call) */

/* Ensure the cache has at least 'needed' elements allocated. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Sequence function: a(n) = sum_{k=1}^{n} floor(k^(1/5))  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* First call: allocate at least n+1 slots and store a(0)=0. */
    if (cache_len == 0)
    {
        ensure_cache((size_t)n + 1);
        a_cache[0] = 0;
        cache_len = 1;
    }
    else
    {
        /* Make sure we have at least n+1 slots */
        if ((size_t)n + 1 > cache_cap)
            ensure_cache((size_t)n + 1);
    }

    /* Compute missing entries up to n. */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        int r = root5_uint64((uint64_t)i);
        a_cache[i] = a_cache[i - 1] + (seq_t)r;
    }
    cache_len = (size_t)n + 1;

    return a_cache[n];
}
