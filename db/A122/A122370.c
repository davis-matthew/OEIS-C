/*
 * A122370  Dimension of 6-variable non-commutative harmonics (twisted
 *          derivative). The dimension of the space of non-commutative
 *          polynomials in 6 variables which are killed by all symmetric
 *          differential operators (where for a monomial w, d_{xi} ( xi w ) =
 *          w and d_{xi} ( xj w ) = 0 for i/=j).
 * https://oeis.org/A122370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 81*a(n-2) + 192*a(n-3) - 189*a(n-4) +
 *            53*a(n-5); initial terms: a(0)=1, a(1)=5, a(2)=29, a(3)=172,
 *            a(4)=1026
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* current capacity of cache        */
    static idx_t   last  = 0;   /* highest index computed so far     */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 5;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* out‑of‑memory fallback (unlikely) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)29;
        cache[3] = (seq_t)172;
        cache[4] = (seq_t)1026;
        last = 4;
    }

    /* if requested index already computed, return it */
    if ((size_t)n < cap)
        return cache[(size_t)n];

    /* grow the cache to accommodate index n */
    size_t new_cap = n + 1;
    if (new_cap > cap) {
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* out‑of‑memory fallback (unlikely) */
        cache = tmp;
        cap = new_cap;
    }

    /* compute any missing terms using the linear recurrence */
    for (idx_t k = last + 1; k <= n; ++k) {
        /* compute positive and negative parts separately
           to avoid underflow on unsigned types                  */
        seq_t pos = (seq_t)15 * cache[k-1]
                 + (seq_t)192 * cache[k-3]
                 + (seq_t)53  * cache[k-5];
        seq_t neg = (seq_t)81  * cache[k-2]
                 + (seq_t)189 * cache[k-4];
        cache[k] = pos - neg;
    }

    last = n;
    return cache[(size_t)n];
}
