/*
 * A025257  (1/3)*s(n+3), where s = A025256.
 * https://oeis.org/A025257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   (n+3)*a(n) + (-2*n-3)*a(n-1) - n*a(n-2) +5*(-2*n+3)*a(n-3)
 *            +(n-3)*a(n-4)=0 Initial values: a(1)=1, a(2)=2, a(3)=6, a(4)=14.
 *            Recurrence suitable for computation: a(n) = ((2*n+3)*a(n-1) +
 *            n*a(n-2) + 5*(2*n-3)*a(n-3) - (n-3)*a(n-4)) / (n+3)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array storing computed a(1) .. a(cache_cap) */
    static idx_t cache_cap = 0;      /* number of terms already computed */

    if (n == 0)                         /* out of offset range – not used */
        return (seq_t)0;

    /* initialise first 4 terms on first use */
    if (!cache) {
        cache = (seq_t*)malloc(sizeof(seq_t) * 4);
        cache_cap = 4;
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)2;   /* a(2) */
        cache[2] = (seq_t)6;   /* a(3) */
        cache[3] = (seq_t)14;  /* a(4) */
    }

    /* if we already have the requested term, return it immediately */
    if (n <= cache_cap)
        return cache[n - 1];   /* cache is 0‑based, indices start at 1 */

    /* extend the cache to compute up to term n */
    {
        idx_t new_cap = n;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        /* compute terms from cache_cap+1 to n */
        for (idx_t idx = cache_cap; idx < new_cap; ++idx) {
            if (idx < 4) {
                /* base cases already stored, nothing to do */
                continue;
            }
            idx_t nn = idx + 1;   /* term index (1‑based) */

            seq_t term1 = ((seq_t)(2 * nn + 3)) * cache[idx - 1];
            seq_t term2 = ((seq_t)(nn)) * cache[idx - 2];
            seq_t term3 = ((seq_t)5) * ((seq_t)(2 * nn - 3)) * cache[idx - 3];
            seq_t term4 = ((seq_t)(nn - 3)) * cache[idx - 4];

            seq_t numerator = term1 + term2 + term3;
            numerator -= term4;
            seq_t denom = (seq_t)(nn + 3);

            cache[idx] = numerator / denom;
        }
        cache_cap = new_cap;
    }

    return cache[n - 1];
}
