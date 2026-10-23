/*
 * A185137  Tri-recursive sequence: a(n) = a(a(a(n-1))) + a(n - a(a(-3+n))) if
 *          n>6, otherwise a(n) = 1.
 * https://oeis.org/A185137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = 1 for n ≤ 6, a(n) = a(a(a(n-1))) + a(n - a(a(n-3))) for n
 *            > 6
 * Range:     u8 n <= 536, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation cache for previously computed terms */
    static seq_t *cache      = NULL;  /* array of computed a(i) values          */
    static idx_t  cache_len  = 0;     /* highest index currently stored (≤ n)   */
    static size_t cache_cap  = 0;     /* allocated capacity (in elements)       */

    /* Protect against calls with n < 1 (offset is 1) */
    if (n == 0) return 0;

    /* Grow the cache if needed */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;          /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                                /* allocation failed */
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Compute all missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 6) {
            val = 1;                                     /* initial values    */
        } else {
            /* a(i-1)             */
            seq_t t1 = cache[(size_t)(i - 2)];
            /* a(a(i-1))          */
            seq_t t2 = cache[(size_t)(t1 - 1)];
            /* a(a(a(i-1)))       */
            seq_t t3 = cache[(size_t)(t2 - 1)];

            /* a(i-3)             */
            seq_t t4 = cache[(size_t)(i - 4)];
            /* a(a(i-3))          */
            seq_t t5 = cache[(size_t)(t4 - 1)];

            /* second addend: a( i - a(a(i-3)) ) */
            idx_t idx = (idx_t)i - (idx_t)t5;  /* idx ≥ 1 */
            seq_t t6 = cache[(size_t)(idx - 1)];

            val = t3 + t6;
        }
        cache[(size_t)(i - 1)] = val;
    }
    cache_len = n;
    return cache[(size_t)(n - 1)];
}
