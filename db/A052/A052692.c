/*
 * A052692  Expansion of e.g.f. (1-x^4)/(1-x-x^4).
 * https://oeis.org/A052692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 1, a(2) = 2, a(3) = 6, a(4) = 24 a(n) =
 *            n*a(n-1) + n*(n-1)*(n-2)*(n-3)*a(n-4) (n ≥ 4) (OEIS A052692)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache         = NULL;
    static size_t cache_cap     = 0;
    static idx_t   computed_max = 0;

    /* initialise cache once */
    if (!cache) {
        cache_cap = 64;                         /* initial capacity */
        cache     = (seq_t*)realloc(cache, sizeof(seq_t)*cache_cap);
        cache[0]  = 1;
        cache[1]  = 1;
        cache[2]  = 2;
        cache[3]  = 6;
        cache[4]  = 24;
        computed_max = 4;
    }

    /* if requested value already computed, return it */
    if (n <= computed_max) return cache[n];

    /* grow cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap <= n) new_cap *= 2;
        cache = (seq_t*)realloc(cache, sizeof(seq_t)*new_cap);
        cache_cap = new_cap;
    }

    /* compute missing values */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t ni = (seq_t)i;
        /* recurrence only applies for i >= 5 (i.e., i > 4) */
        seq_t term = ni * cache[i-1];
        term += ni * (seq_t)(i-1) * (seq_t)(i-2) * (seq_t)(i-3) * cache[i-4];
        cache[i] = term;
    }
    computed_max = n;
    return cache[n];
}
