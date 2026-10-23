/*
 * A165873  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A165873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   _num_coeff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A165873 – Number of reduced words of length n in the Coxeter group
 *  on 13 generators S_i with relations S_i^2 = (S_i S_j)^10 = I.
 *
 *  Ordinary generating function:
 *      N(t) / D(t) where
 *          N(t) = t^10 + 2*t^9 + 2*t^8 + 2*t^7 + 2*t^6 + 2*t^5
 *                 + 2*t^4 + 2*t^3 + 2*t^2 + 2*t + 1
 *          D(t) = 66*t^10 - 11*t^9 - 11*t^8 - 11*t^7 - 11*t^6
 *                 - 11*t^5 - 11*t^4 - 11*t^3 - 11*t^2 - 11*t + 1
 *
 *  From D(t)*G(t)=N(t) we obtain for all n≥0
 *          a(n) = N_n + 11*(a(n-1)+…+a(n-9)) - 66*a(n-10)
 *  where N_n = 1 if n==0,
 *             2 if 1≤n≤9,
 *             1 if n==10,
 *             0 otherwise.
 *
 *  The function keeps a static cache so that successive calls in any
 *  order are answered in O(1) after the needed values have been
 *  computed once.
 */

#include <stdlib.h>
#include "../utils/_num_coeff.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* if we already have the value, return it */
    if (n < cache_size)
        return cache[n];

    /* constants used in the recurrence */
    const seq_t c11 = (seq_t)11;
    const seq_t c66 = (seq_t)66;

    /* grow cache to accommodate up to n */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    if (!new_cache)
    {
        /* allocation failed – return 0 (no error handling specified) */
        return 0;
    }
    cache = new_cache;

    /* compute missing terms from cache_size up to n */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        seq_t sum1 = (seq_t)0;      /* sum of a(i-1) .. a(i-9) */
        for (idx_t j = 1; j <= 9 && j <= i; ++j)
            sum1 += cache[i - j];

        seq_t sum2 = (i >= 10) ? cache[i - 10] : (seq_t)0;

        cache[i] = _num_coeff(i) + c11 * sum1 - c66 * sum2;
    }

    cache_size = new_size;
    return cache[n];
}
