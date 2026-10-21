/*
 * A097678  E.g.f.: (1/(1-x^3))*exp( 3*sum_{i>=0} x^(3*i+2)/(3*i+2) ) for an
 *          order-3 linear recurrence with varying coefficients.
 * https://oeis.org/A097678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(n-1)*a(n-2) + n!/(n-3)!*a(n-3); Initial values:
 *            a(0)=1, a(1)=0, a(2)=3 (n!/(n-3)!) = n*(n-1)*(n-2) for n>=3
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for already computed values */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated size */
    static idx_t sz    = 0;   /* number of filled elements */

    /* initialise cache with the first three terms */
    if (cache == NULL) {
        cap = 4;               /* start with small capacity */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failed – abort silently */
        cache[0] = (seq_t)1;  /* a(0) */
        cache[1] = (seq_t)0;  /* a(1) */
        cache[2] = (seq_t)3;  /* a(2) */
        sz = 3;
    }

    /* extend cache if needed */
    if (n >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;   /* grow geometrically */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;           /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing values up to n */
    if (n >= sz) {
        for (idx_t i = sz; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                val = (seq_t)1;
            } else if (i == 1) {
                val = (seq_t)0;
            } else if (i == 2) {
                val = (seq_t)3;
            } else { /* recurrence for i >= 3 */
                seq_t fac = (seq_t)i * (seq_t)(i - 1) * (seq_t)(i - 2); /* n!/(n-3)! */
                val = (seq_t)3 * (seq_t)(i - 1) * cache[i - 2]
                      + fac * cache[i - 3];
            }
            cache[i] = val;
        }
        sz = n + 1;
    }

    return cache[n];
}
