/*
 * A061984  a(n) = 1 + a([n/2]) + a([n/3]) with a(0) = 0.
 * https://oeis.org/A061984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 647, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; a(n)=1+a(n/2)+a(n/3) */
#include <stdlib.h>

static seq_t *cache = NULL;           /* memoized values */
static unsigned char *filled = NULL;  /* 0=unset, 1=set */
static idx_t cache_cap = 0;           /* size of cache arrays */

/* Ensure cache arrays can hold index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;
    idx_t new_cap = (n + 1) * 2;
    /* reallocate cache */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    unsigned char *new_filled = (unsigned char *)realloc(filled, new_cap * sizeof(unsigned char));
    if (!new_cache || !new_filled) {
        /* allocation failed: terminate */
        abort();
    }
    /* zero-initialize new part of filled (unset) */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        new_filled[i] = 0;
    cache = new_cache;
    filled = new_filled;
    cache_cap = new_cap;
}

/* Recursive computation with memoization */
static seq_t a_recursive(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n < cache_cap && filled[n])
        return cache[n];
    seq_t res = (seq_t)1
                + a_recursive(n / 2)
                + a_recursive(n / 3);
    if (n >= cache_cap)
        ensure_cache(n);
    cache[n] = res;
    filled[n] = 1;
    return res;
}

seq_t a(idx_t n)
{
    if (n >= cache_cap)
        ensure_cache(n);
    return a_recursive(n);
}
