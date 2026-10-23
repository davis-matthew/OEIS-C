/*
 * A015547  Expansion of x/(1 - 5*x - 11*x^2).
 * https://oeis.org/A015547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1, a(n) = 5*a(n-1) + 11*a(n-2) for n ≥ 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow arbitrary call order */
    static seq_t *cache = NULL;   /* array of already computed terms */
    static idx_t  last  = 0;      /* highest index computed so far */
    static idx_t  cap   = 0;      /* current allocation size */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 2;                       /* allocate for indices 0 and 1 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* fail silently if out of memory */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        last = 1;
    }

    /* grow cache if necessary */
    if (n >= cap) {
        idx_t newCap = n + 1;
        seq_t *newCache = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
        if (!newCache) return 0;       /* fail silently if out of memory */
        cache = newCache;
        cap = newCap;
    }

    /* compute missing terms up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = 5 * cache[i - 1] + 11 * cache[i - 2];
        }
        last = n;
    }

    return cache[n];
}
