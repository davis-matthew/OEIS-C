/*
 * A170841  G.f.: Product_{k>=0} (1 + 2x^(2^k-1) + 3x^(2^k)).
 * https://oeis.org/A170841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of ∏_{k≥0} (1 + 2x^{2^k-1} + 3x^{2^k}) at x^n
 * Range:     u8 n <= 12, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Allocate two arrays big enough to hold the
       convolution results up to degree n.  Casting the
       void* returned by calloc makes the code compile
       in both C and C++.  The arrays are treated as
       plain sequences of seq_t and are initialized
       explicitly. */
    seq_t *dp    = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *next  = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp || !next) {
        free(dp);
        free(next);
        return (seq_t)0;        /* graceful failure */
    }

    /* Initialise dp to represent the product of no
       factors yet: dp[0] = 1, all other coefficients
       are 0. */
    for (idx_t i = 0; i <= n; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Iteratively multiply by each factor
       (1 + 2x^{2^k-1} + 3x^{2^k}) while the
       lowest exponent 2^k‑1 does not exceed n. */
    for (idx_t pow = 1; pow <= n + 1; pow <<= 1) {   /* pow == 2^k */
        /* Clear the next array */
        for (idx_t i = 0; i <= n; ++i) next[i] = (seq_t)0;

        idx_t exp1 = pow - 1;   /* exponent of the 2‑term */
        idx_t exp2 = pow;       /* exponent of the 3‑term */

        for (idx_t i = 0; i <= n; ++i) {
            if (dp[i] == (seq_t)0) continue;   /* skip zero terms */

            /* constant term */
            next[i] += dp[i];

            /* 2 * x^{exp1} term */
            if (i + exp1 <= n) {
                next[i + exp1] += (seq_t)2 * dp[i];
            }

            /* 3 * x^{exp2} term */
            if (i + exp2 <= n) {
                next[i + exp2] += (seq_t)3 * dp[i];
            }
        }

        /* Prepare for next iteration: dp <- next, next reused */
        seq_t *tmp = dp; dp = next; next = tmp;
    }

    seq_t result = dp[n];
    free(dp);
    free(next);
    return result;
}
