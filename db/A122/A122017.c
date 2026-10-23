/*
 * A122017  a(n) = if n even then a(n - 1) - (n - 1)*a(n - 2) otherwise 2*(a(n
 *          - 1) + (n - 1)*a(n - 2)).
 * https://oeis.org/A122017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   if n even then a(n) = a(n-1)-(n-1)*a(n-2); else a(n) =
 *            2*(a(n-1)+(n-1)*a(n-2)); Initial values: a(0)=1, a(1)=2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, NULL */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t  cache_len = 0;      /* number of stored elements (0 .. cache_len-1) */
    static idx_t  cache_cap = 0;      /* allocated capacity                         */

    /* initialise cache with the two first terms */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;   /* out of memory, return 0 as a safe fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_len = 2;
    }

    /* already computed ? */
    if (n < cache_len) {
        return cache[n];
    }

    /* compute values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* grow cache if necessary */
        if (i >= cache_cap) {
            idx_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) return 0;   /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* recurrence */
        const seq_t prev1 = cache[i - 1];
        const seq_t prev2 = cache[i - 2];
        const seq_t mult  = (seq_t)(i - 1) * prev2;   /* (n-1)*a(n-2) */

        if ((i & 1U) == 0) {                /* even i */
            cache[i] = prev1 - mult;
        } else {                            /* odd i */
            cache[i] = (prev1 + mult) * (seq_t)2;
        }
    }

    cache_len = n + 1;
    return cache[n];
}
