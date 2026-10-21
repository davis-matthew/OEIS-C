/*
 * A097804  a(n) = 3*(2*5^n + 1).
 * https://oeis.org/A097804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(2*5^n + 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The program uses the linear recurrence: a(0)=9, a(1)=33, a(n)=6*a(n-1)-5*a(n-2) */
/* It memoises computed values so each n is evaluated only once. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of values stored (next free index) */
    static size_t cache_cap = 0;   /* capacity of the cache array */

    /* Initialise the cache to hold the first two terms. */
    if (cache_len == 0) {
        /* start with room for at least two terms */
        cache_cap = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)9;   /* a(0) */
        cache[1] = (seq_t)33;  /* a(1) */
        cache_len = 2;
    }

    /* If the requested term is already in cache, return it immediately. */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Ensure the cache has enough capacity for all indices up to n. */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 2;
        while (new_cap < needed) new_cap <<= 1;    /* double until enough */
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* Compute any missing terms using the recurrence. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t term1 = cache[i-1] * (seq_t)6;
        seq_t term2 = cache[i-2] * (seq_t)5;
        cache[i] = term1 - term2;
    }
    cache_len = needed;

    return cache[n];
}
