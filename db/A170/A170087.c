/*
 * A170087  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 28 * sum_{k=1}^{min(36,n)} a(n-k) + (n==0?1:2) for n<37 *
 *            a(n) = 28 * sum_{k=1}^{36} a(n-k) - 406 * a(n-37) + (n==37?1:0)
 *            for n>=37 * This is derived from the generating function * G(t)
 *            = (t^37 + 2*t^36 + … + 2*t + 1) / (1 - 28*t - … - 28*t^36 +
 *            406*t^37)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;     /* allocated capacity */
    static idx_t   maxc  = 0;     /* largest index computed */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 64;                           /* start with 64 entries, ample for n ≤ 63 */
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        cache[0] = (seq_t)1;                /* a(0) = 1 */
        maxc = 0;
    }

    /* grow cache to accommodate n */
    if ((idx_t)n >= cap) {
        while ((idx_t)n >= cap) cap <<= 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* compute missing terms up to n */
    for (idx_t i = maxc + 1; i <= n; ++i) {
        seq_t sum = 0;

        if (i < 37) {                     /* recurrence for 0 ≤ i < 37 */
            for (idx_t k = 1; k <= i; ++k)
                sum += cache[i - k];       /* sum of a(i-1), … , a(0) */
            seq_t val = 28 * sum;
            val += (i == 0 ? (seq_t)1 : (seq_t)2);
            cache[i] = val;
        } else {                           /* recurrence for i ≥ 37 */
            for (idx_t k = 1; k <= 36; ++k)
                sum += cache[i - k];       /* sum of a(i-1) … a(i-36) */
            seq_t val = 28 * sum;
            val -= (seq_t)406 * cache[i - 37];
            if (i == 37)                    /* extra +1 for the t^37 term */
                val += (seq_t)1;
            cache[i] = val;
        }
    }

    maxc = n;
    return cache[n];
}
