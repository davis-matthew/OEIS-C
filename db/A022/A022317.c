/*
 * A022317  a(n) = a(n-1) + a(n-2) + 1, with a(0) = 0 and a(1) = 12.
 * https://oeis.org/A022317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 1 with a(0) = 0, a(1) = 12 (OEIS
 *            A022317)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache      = NULL;
    static idx_t   cache_cap = 0;
    static idx_t   last_val  = 0;
    static bool    init      = false;

    /* initialise the cache with the first two terms */
    if (!init) {
        /* start with space for 2 elements */
        cache_cap = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)12;
        last_val  = 1;
        init = true;
    }

    /* if the requested index is already computed, return it */
    if (n <= last_val)
        return cache[n];

    /* enlarge the cache if necessary */
    if ((size_t)n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while ((size_t)n >= new_cap)
            new_cap *= 2;          /* exponential growth */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute all missing terms up to n */
    for (idx_t i = last_val + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 2] + (seq_t)1;
    }
    last_val = n;
    return cache[n];
}
