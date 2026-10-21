/*
 * A052966  Expansion of (1-x)/(1-x-4x^2+2x^3).
 * https://oeis.org/A052966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1,  a(1) = 0,  a(2) = 4,
   a(n+3) = 4*a(n+1) + a(n+2) – 2*a(n)   (n ≥ 0)
   Equivalently, 2*a(n) – 4*a(n+1) – a(n+2) + a(n+3) = 0. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;   /* number of values stored (>= cache_cap) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initialise base values once */
    if (cache_len == 0) {
        cache_cap = 8;
        cache     = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;   /* a(0) */
        cache[1] = 0;   /* a(1) */
        cache[2] = 4;   /* a(2) */
        cache_len = 3;
    }

    /* if requested index exceeds current capacity, grow buffer */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* extend cache up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 3) continue;               /* base values already set */
        /* use recurrence: a[i] = 4*a[i-2] + a[i-1] – 2*a[i-3] */
        cache[i] = 4 * cache[i-2] + cache[i-1] - 2 * cache[i-3];
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
