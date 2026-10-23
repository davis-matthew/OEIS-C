/*
 * A021456  Decimal expansion of 1/452.
 * https://oeis.org/A021456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = (10^{n+1} div 452) mod 10 * (digit of the decimal
 *            expansion of 1/452 after n positions)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* digits cache */
static size_t cache_cap = 0;     /* allocated capacity of the cache */
static size_t cache_len = 0;     /* number of digits currently stored */
static uint64_t rem = 1;         /* remainder for the long division */

/* Ensure the cache has room for at least 'needed' elements */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* If we already have the result cached, return it immediately */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Grow the cache to accommodate the requested index */
    grow_cache((size_t)n + 1);

    /* Generate digits up to index n using long division */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        rem *= 10;                         /* shift remainder */
        seq_t digit = rem / 452;            /* next decimal digit */
        rem   %= 452;                       /* new remainder */
        cache[i] = digit;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
