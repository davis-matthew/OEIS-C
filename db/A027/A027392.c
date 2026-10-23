/*
 * A027392  Number of labeled servers of dimension 5.
 * https://oeis.org/A027392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A027392:
   EGF: exp( (x*(300 + 300*x + 200*x^2 + 75*x^3 + 12*x^4))/60 )
   Recurrence: a(0)=1
   a(n) = Σ_{j=1}^{min(n,5)} binom(n-1, j-1) * a(n-j) * c_j
   with c = {0,5,10,20,30,24} */

#include <stdlib.h>

seq_t a(idx_t n) {
    static const seq_t C[6] = {0, 5, 10, 20, 30, 24};    /* connected component counts */

    /* Allocate temporary array for dp[0..n] */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;   /* memory allocation failed */
    dp[0] = 1;

    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = 0;
        idx_t max_j = (m < 5) ? m : 5;
        for (idx_t j = 1; j <= max_j; ++j) {
            /* binomial coefficient binom(m-1, j-1) for small j-1 (0..4) */
            seq_t bin;
            switch (j - 1) {
                case 0: bin = 1; break;
                case 1: bin = (seq_t)(m - 1); break;
                case 2: bin = (seq_t)(m - 1) * (m - 2) / 2; break;
                case 3: bin = (seq_t)(m - 1) * (m - 2) * (m - 3) / 6; break;
                case 4: bin = (seq_t)(m - 1) * (m - 2) * (m - 3) * (m - 4) / 24; break;
                default: bin = 0; break;   /* should never happen */
            }

            seq_t term = dp[m - j] * C[j];
            term = term * bin;
            sum += term;
        }
        dp[m] = sum;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
