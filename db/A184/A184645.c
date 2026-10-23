/*
 * A184645  Number of partitions of n having no parts with multiplicity 10.
 * https://oeis.org/A184645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 128, u64 n <= 418,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A184645
   Number of partitions of n having no parts with multiplicity 10.

   Computation: dynamic programming on the generating function
   Product_{j>0} (1 - x^(10*j) + x^(11*j))/(1 - x^j)

   For each part size j we maintain an array dp[k] that holds the
   coefficient of x^k after all part sizes < j have been processed.
   Updating for part size j uses the recurrence

   S[k] = sum_{t=0}^{⌊k/j⌋} dp[k - t*j]
   dp_new[k] = S[k] - (k ≥ 10*j ? dp[k-10*j] : 0)

   where S is computed incrementally as
   S[k] = (k < j) ? dp[k] : S[k-j] + dp[k].
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    size_t sz = (size_t)(n + 1);
    seq_t *dp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp) return 0;
    dp[0] = 1;                           /* empty partition */

    for (idx_t j = 1; j <= n; ++j) {
        seq_t *newdp = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!newdp) { free(dp); return 0; }

        seq_t *S = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!S) { free(dp); free(newdp); return 0; }

        for (idx_t k = 0; k <= n; ++k) {
            if (k < j)
                S[k] = dp[k];
            else
                S[k] = S[k - j] + dp[k];

            seq_t val = S[k];
            if (k >= 10 * j)
                val -= dp[k - 10 * j];

            newdp[k] = val;
        }

        free(dp);
        free(S);
        dp = newdp;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
