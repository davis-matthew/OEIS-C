/*
 * A128563  Column 2 of triangle A128562.
 * https://oeis.org/A128563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^(2*n+6) in the q-binomial coefficient
 *            [2*n+5, n+2]. Computation via multiplicative update for the
 *            q-binomial polynomial. The algorithm runs in O(n^2) time and
 *            uses O(n) memory
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Parameters describing the q-binomial [2*n+5, n+2] */
    const idx_t K = n + 2;            /* number of parts (rows)        */
    const idx_t L = n + 3;            /* size of each part (columns)   */
    const idx_t m = 2 * n + 6;        /* exponent we need a coefficient for */

    const size_t sz = (size_t)m + 1;  /* array size for polynomial coefficients */

    /* Dynamic arrays for current and next stage */
    seq_t *dp   = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *next = (seq_t *)malloc( sz * sizeof(seq_t) );
    if (!dp || !next) { /* allocation failed – leave unsatisfied */
        return 0;
    }

    dp[0] = (seq_t)1;   /* initialise polynomial = 1 */

    for (idx_t i = 1; i <= K; ++i) {
        /* 1. Build c1[s] = sum_{t>=0} dp[s - i*t] into 'next' */
        next[0] = dp[0];
        for (size_t s = 1; s < sz; ++s) {
            next[s] = dp[s];
            if (s >= (size_t)i) {
                next[s] += next[s - i];
            }
        }

        /* 2. Subtract portions shifted by (L + i)  */
        const size_t shift = (size_t)L + (size_t)i;
        for (size_t s = shift; s < sz; ++s) {
            next[s] -= next[s - shift];
        }

        /* 3. Swap dp and next buffers for next iteration */
        seq_t *tmp  = dp;
        dp         = next;
        next       = tmp;
    }

    seq_t result = dp[m];
    free(dp);
    free(next);
    return result;
}
