/*
 * A195255  O.g.f.: Sum_{n>=0} 3*(n+3)^(n-1)*x^n/(1+n*x)^n.
 * https://oeis.org/A195255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = (n-1)*a(n-1) + 3^n for n >= 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  cache_len  = 0;  /* number of computed terms (next free index) */
    static seq_t pow3         = 1; /* 3^(i-1) for the next term to be scored,
                                     3^0 when cache_len == 1 */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]  = 1;                 /* a(0) */
        cache_len = 1;                 /* next free index is 1 */
        pow3      = 1;                 /* 3^0 */
    }

    /* trivial case */
    if (n == 0)
        return cache[0];

    /* enlarge the cache if needed */
    if (cache_len <= n) {
        size_t needed = n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap;
            if (new_cap == 0) new_cap = 16;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            pow3 = pow3 * 3;                    /* now pow3 = 3^i */
            cache[i] = ((seq_t)(i - 1)) * cache[i - 1] + pow3;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
