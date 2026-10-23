/*
 * A126499  Number of base 31 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   For n ≥ 1 let dp[d] be the number of base‑31 strings of length n ending
   with digit d (0 ≤ d ≤ 30), where leading zeros are allowed.
   Initialization (n = 1):
       dp[d] = 1  for all d
   Transition (n > 1):
       newdp[d] = Σ dp[k]  over k = max(0, d-3) … min(30, d+3)
   Result:
       a(n) = Σ dp[d]  over d = 0 … 30
*/

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* temporary arrays for DP */
    seq_t dp[31];
    seq_t ndp[31];

    /* n = 1 case: all digits 0..30 are allowed */
    for (int d = 0; d <= 30; ++d) {
        dp[d] = (seq_t)1;
    }

    /* iterate for positions 2 .. n */
    for (idx_t pos = 2; pos <= n; ++pos) {
        for (int d = 0; d <= 30; ++d) {
            int low  = d - 3;
            int high = d + 3;
            if (low  < 0)  low  = 0;
            if (high > 30) high = 30;
            seq_t sum = (seq_t)0;
            for (int k = low; k <= high; ++k) {
                sum += dp[k];
            }
            ndp[d] = sum;
        }
        /* copy ndp back to dp for next iteration */
        for (int d = 0; d <= 30; ++d) {
            dp[d] = ndp[d];
        }
    }

    /* sum all dp[d] to obtain a(n) */
    seq_t total = (seq_t)0;
    for (int d = 0; d <= 30; ++d) {
        total += dp[d];
    }
    return total;
}
