/*
 * A163350  a(n) = 8*a(n-1) - 14*a(n-2) for n > 1; a(0) = 1, a(1) = 6.
 * https://oeis.org/A163350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 14*a(n-2), a(0) = 1, a(1) = 6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*
 * Compute the nth term of OEIS sequence A163350.
 * The function is memoized: subsequent calls reuse previously
 * computed values and work in O(n) overall time.
 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached values */
    static idx_t cache_used = 0;  /* number of cached values */
    static idx_t cache_cap  = 0;  /* capacity of cache array */

    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* if we already have the value cached, return it */
    if (cache && n < cache_used)
        return cache[n];

    /* initialise cache if it doesn't exist yet */
    if (!cache) {
        cache_cap = 2;                         /* need space for a(0) and a(1) */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                   /* fatal: out‑of‑memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache_used = 2;
    }

    /* grow the cache to accommodate a(n) */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;    /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to a(n) */
    for (idx_t i = cache_used; i <= n; ++i) {
        cache[i] = 8 * cache[i - 1] - 14 * cache[i - 2];
    }
    cache_used = n + 1;

    return cache[n];
}
