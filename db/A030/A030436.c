/*
 * A030436  Expansion of g.f. (1 + x - 2*x^2 - x^3)/(1 - 4*x^2 + 2*x^4).
 * https://oeis.org/A030436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) - 2*a(n-4), a(0)=1, a(1)=1, a(2)=2, a(3)=3
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 37, u64 n <= 73, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values, grows as needed */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t last_computed = 0;   /* largest index already computed */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* make room for at least first 4 terms */
        cache_cap = (n > 3) ? n + 1 : 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        last_computed = 3;
    }

    /* grow cache if the requested index is beyond current capacity */
    if (n + 1 > cache_cap) {
        cache_cap = n + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* if the value is already known, return it directly */
    if (n <= last_computed)
        return cache[n];

    /* compute missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* recurrence: a(i) = 4*a(i-2) - 2*a(i-4) */
        seq_t term1 = (seq_t)4 * cache[i - 2];
        seq_t term2 = (seq_t)2 * cache[i - 4];
        cache[i] = term1 - term2;
    }

    last_computed = n;
    return cache[n];
}
