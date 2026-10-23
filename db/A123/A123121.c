/*
 * A123121  Length of the n-th Zimin word (A082215(n)).
 * https://oeis.org/A123121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1, a(n) = 2*a(n-1) + ceil(log10(n+1)) for n >= 2
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   ceil_log10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ceil_log10.h"

/* Cache storage for previously computed values. */
static seq_t *cache      = NULL; /* 1‑based array: cache[1] = a(1) = 1 */
static idx_t  cache_cap  = 0;    /* current allocated capacity (>= 1) */
static idx_t  cached_n   = 0;    /* highest n that has been computed */

/* Grow the cache until it can hold indices up to 'needed' (inclusive). */
static void grow_cache(idx_t needed)
{
    if (needed <= cache_cap) return;

    idx_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();           /* allocation failed */
    cache     = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n < 1 */

    /* Ensure cache is initialized. */
    if (!cache) {
        grow_cache(2);                /* at least index 1 must be valid */
        cache[1] = 1;
        cached_n = 1;
    }

    /* Extend cache beyond n if required. */
    if (n > cached_n) {
        grow_cache(n + 1);            /* indices 1 … n must be available */

        for (idx_t i = cached_n + 1; i <= n; ++i) {
            seq_t term = (seq_t)2 * cache[i - 1] + ceil_log10(i + 1);
            cache[i] = term;
        }
        cached_n = n;
    }

    return cache[n];
}
