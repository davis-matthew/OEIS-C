/*
 * A126410  Number of base 23 n-digit numbers with adjacent digits differing
 *          by two or less.
 * https://oeis.org/A126410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   a(1) = 23
   For n ≥ 2 let dp[d] be the number of base‑23 n‑digit numbers
   ending with digit d (0 ≤ d ≤ 22) and having adjacent digits
   differing by at most two.
   Then
      dp_next[d] = Σ_{k=max(0,d-2)}^{min(22,d+2)} dp[k]
   The total a(n) = Σ_{d=0}^{22} dp[d].
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle base cases */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)23;

    /* DP arrays for the current and next length */
    seq_t dp[23];
    seq_t ndp[23];

    /* start with length 1: one number ending with each digit */
    for (int d = 0; d < 23; ++d)
        dp[d] = (seq_t)1;

    /* iterate until we reach length n */
    for (idx_t step = 2; step <= n; ++step) {
        /* clear next array */
        for (int d = 0; d < 23; ++d)
            ndp[d] = (seq_t)0;

        /* compute transitions */
        for (int d = 0; d < 23; ++d) {
            int k_start = d - 2; if (k_start < 0)        k_start = 0;
            int k_end   = d + 2; if (k_end > 22)         k_end = 22;
            seq_t sum = (seq_t)0;
            for (int k = k_start; k <= k_end; ++k)
                sum += dp[k];
            ndp[d] = sum;
        }

        /* copy back to dp for next iteration */
        for (int d = 0; d < 23; ++d)
            dp[d] = ndp[d];
    }

    /* total number of length-n strings */
    seq_t total = (seq_t)0;
    for (int d = 0; d < 23; ++d)
        total += dp[d];

    return total;
}
