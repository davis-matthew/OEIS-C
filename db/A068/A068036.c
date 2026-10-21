/*
 * A068036  Number of subsets of {1,2,3,...,n} that sum to 0 mod 15.
 * https://oeis.org/A068036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: number of subsets of {1,2,…,n} whose sum is 0 modulo 15.
   The dynamic programming recurrence is:
        dp_new[r] = dp_old[r] + dp_old[(r - i mod 15 + 15) mod 15],
   where i runs from 1 to n.  dp[0] starts at 1 (empty subset).  The
   array size is 15, so each step costs O(15) and the whole algorithm
   runs in O(n) time. */
seq_t a(idx_t n)
{
    seq_t dp[15] = {0};
    seq_t newdp[15];
    dp[0] = 1;

    for (idx_t i = 1; i <= n; ++i) {
        int r5 = i % 15;        /* shift caused by element i */

        /* copy current dp to newdp (will be updated in place) */
        for (int r = 0; r < 15; ++r)
            newdp[r] = dp[r];

        /* add contribution of subsets that include i */
        for (int r = 0; r < 15; ++r) {
            int src = r - r5;
            if (src < 0)
                src += 15;
            newdp[r] += dp[src];
        }

        /* move newdp back to dp for next iteration */
        for (int r = 0; r < 15; ++r)
            dp[r] = newdp[r];
    }

    return dp[0];
}
