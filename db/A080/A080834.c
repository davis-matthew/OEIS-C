/*
 * A080834  E.g.f. exp( x/(1 - x - x^2) ) / (1 - x - x^2).
 * https://oeis.org/A080834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n*a(n-1) + (n-1)^2*a(n-2) * - 2*(n-2)^2*(n-1)*a(n-3) *
 *            - (n-3)*(n-2)^2*(n-1)*a(n-4) * with a(0)=1, a(1)=2, a(2)=9,
 *            a(3)=58
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;   /* last index computed */

    /* initialise base cases on first call */
    if (cache == NULL) {
        cache = (seq_t*)malloc(4 * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)58;
        max_computed = 3;
    }

    /* return cached value if already computed */
    if (n <= max_computed)
        return cache[n];

    /* grow cache to hold up to index n */
    if (n + 1 > (idx_t)(0 ? 0 : 1)) { /* avoid lint warning for unused variable */
        if (n + 1 > (idx_t)(-1)) return (seq_t)0; /* overflow guard */
    }
    if (n + 1 > (idx_t)0) { /* dummy block  */
        if (n + 1 > (idx_t)(-1)) return (seq_t)0;
    }
    /* real growth logic */
    {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failed */
        cache = tmp;
    }

    /* compute missing values up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t term1 = ((seq_t)2) * ((seq_t)i) * cache[i - 1];
        seq_t term2 = ((seq_t)(i - 1)) * ((seq_t)(i - 1)) * cache[i - 2];
        seq_t term3 = ((seq_t)2) * ((seq_t)(i - 2)) * ((seq_t)(i - 2))
                     * ((seq_t)(i - 1)) * cache[i - 3];
        seq_t term4 = ((seq_t)(i - 3)) * ((seq_t)(i - 2)) * ((seq_t)(i - 2))
                     * ((seq_t)(i - 1)) * cache[i - 4];
        cache[i] = term1 + term2 - term3 - term4;
    }
    max_computed = n;

    return cache[n];
}
