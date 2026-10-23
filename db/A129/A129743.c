/*
 * A129743  a(n) = -(u^n-1)*(v^n-1) with u = 2+sqrt(3), v = 2-sqrt(3).
 * https://oeis.org/A129743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = -(u^n-1)*(v^n-1) where u = 2+√3, v = 2-√3 Recurrence:
 *            a(1)=2, a(2)=12, a(3)=50, a(n) = 5*a(n-1) - 5*a(n-2) + a(n-3) (n
 *            ≥ 4) Implementation caches computed values so that calls in any
 *            order are O(1) per new term. Uses only integer arithmetic
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1 – value for n=0 is irrelevant */

    /* static cache of computed terms */
    static seq_t *cache   = NULL;   /* array indexed from 0 to max_n (0 unused) */
    static size_t  cap    = 0;       /* allocated capacity (number of elements)   */
    static idx_t   max_n  = 0;       /* highest n already computed in cache       */

    /* required array size up to index n (inclusive) */
    size_t need = (size_t)n + 1;     /* we need indices 0..n */

    /* first call: allocate space and store the three base terms */
    if (cache == NULL) {
        if (need < 4) need = 4;          /* at least room for indices 1..3 */
        cache = (seq_t*)malloc(need * sizeof(seq_t));
        if (!cache) abort();            /* fatal error if allocation fails */

        cap = need;
        cache[0] = 0;                    /* unused */
        cache[1] = 2;
        cache[2] = 12;
        cache[3] = 50;
        max_n = 3;
    }
    /* grow the cache if the requested n is larger than the current capacity */
    else if (need > cap) {
        seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = need;
    }

    /* compute any missing terms up to n */
    if (n > max_n) {
        for (idx_t k = max_n + 1; k <= n; ++k) {
            /* recurrence: a(k) = 5*a(k-1) - 5*a(k-2) + a(k-3) */
            cache[(size_t)k] = 5 * cache[(size_t)(k-1)] - 5 * cache[(size_t)(k-2)] + cache[(size_t)(k-3)];
        }
        max_n = n;
    }

    return cache[(size_t)n];
}
