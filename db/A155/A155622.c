/*
 * A155622  a(n) = 11^n - 2^n + 1.
 * https://oeis.org/A155622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n - 2^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 10, a(n) = 13*a(n-1) - 22*a(n-2) + 10 for n>1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;           /* cached values */
    static idx_t cache_cap = 0;           /* allocated capacity (in elements) */
    static idx_t cache_len = 0;           /* number of values already computed */

    /* Grow the cache if necessary */
    if (cache_cap <= n) {
        idx_t new_cap = n + 1;
        if (cache == NULL)
            cache = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (cache == NULL)          /* allocation failure – give up */
            return (seq_t)0;
        cache_cap = new_cap;
    }

    /* Initialise the first two terms once */
    if (cache_len == 0) {
        /* Ensure at least two slots exist */
        if (cache_cap < 2) {
            idx_t new_cap = 2;
            seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
            if (tmp == NULL)
                return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache_len = 2;
    }

    /* Return cached value if we already have it */
    if (n < cache_len)
        return cache[(size_t)n];

    /* Compute terms up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        size_t ii = (size_t)i;
        seq_t term = cache[ii - 1] * (seq_t)13
                   - cache[ii - 2] * (seq_t)22
                   + (seq_t)10;
        cache[ii] = term;
    }
    cache_len = n + 1;
    return cache[(size_t)n];
}
