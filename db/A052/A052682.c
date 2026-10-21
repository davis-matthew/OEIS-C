/*
 * A052682  Expansion of e.g.f. (1-x)/(1-x-3*x^2).
 * https://oeis.org/A052682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=0, a(n)=n*a(n-1)+3*n*(n-1)*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache holding already computed values */
    static seq_t *cache   = NULL;  /* array of a(n) values */
    static idx_t  cache_cap = 0;   /* allocated size */
    static idx_t  last_computed = (idx_t)-1;  /* largest n computed */

    /* initialise cache with first two terms */
    if (cache == NULL) {
        /* Allocate a small initial capacity */
        cache_cap = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;            /* out of memory; caller won't use this */
        cache[0] = 1;                    /* a(0) */
        cache[1] = 0;                    /* a(1) */
        last_computed = 1;
    }

    /* Return value if it was already computed */
    if (n <= last_computed) {
        return cache[n];
    }

    /* Extend cache if needed */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* out of memory */
        cache   = tmp;
        cache_cap = new_cap;
    }

    /* Compute values from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sn = (seq_t)i;
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t term1 = sn * prev1;
        seq_t coef  = ((seq_t)3 * sn * (seq_t)(i - 1));
        seq_t term2 = coef * prev2;
        cache[i] = term1 + term2;
    }

    last_computed = n;
    return cache[n];
}
