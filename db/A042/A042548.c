/*
 * A042548  Numerators of continued fraction convergents to sqrt(803).
 * https://oeis.org/A042548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(803) */
/* Computation: P_(-1)=1, P_0=28, P_n = a_n*P_(n-1)+P_(n-2)
   where the continued fraction coefficients are
   a_0=28, and for n>=1 the periodic part is
   [2,1,27,1,2,56] (period length 6). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* periodic coefficients after the initial term */
    static const uint64_t period[6] = {2, 1, 27, 1, 2, 56};
    static seq_t *cache = NULL;   /* cache of computed numerators   */
    static idx_t last   = 0;       /* highest index already computed */

    /* initialise cache with the first term (a(0)=28) */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)28;
        last = 0;
    }

    /* already computed? */
    if (n <= last)
        return cache[n];

    /* grow cache to hold up to index n */
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!cache) abort();

    /* compute new terms in order */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1) {
            /* P_1 = a_1*P_0 + P_(−1)  with P_(−1)=1 */
            cache[i] = (seq_t)period[0] * cache[i - 1] + (seq_t)1;
        } else {
            const seq_t coeff = (seq_t)period[(i - 1) % 6];
            cache[i] = coeff * cache[i - 1] + cache[i - 2];
        }
    }
    last = n;
    return cache[n];
}
