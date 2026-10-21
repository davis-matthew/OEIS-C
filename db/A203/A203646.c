/*
 * A203646  Number of arrays of n 0..15 integers with new values introduced in
 *          order 0..15 but otherwise unconstrained.
 * https://oeis.org/A203646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   Let dp[k] (0 ≤ k ≤ 15) be the number of arrays of the current
   length whose largest used value equals k.
   Base (length 1):  dp[0] = 1, all others 0.
   Transition for each subsequent length L (L ≥ 2):
        newdp[0] = dp[0];                            // only value 0 can be used
        for k = 1 … 15:
            newdp[k] = dp[k] * (k+1) + dp[k-1];      // repeat any 0..k or introduce k
   After processing the desired length n, a(n) = Σ_{k=0}^{15} dp[k]. */

seq_t a(idx_t n)
{
    if (n == 0)   /* defined only for n ≥ 1; return 0 for safety */
        return 0;

    /* dp stores counts for the current length; newdp stores counts for the next length. */
    seq_t dp[16]  = {0};
    seq_t newdp[16] = {0};

    dp[0] = 1; /* length 1: only [0] */

    for (idx_t len = 2; len <= n; ++len) {
        /* k = 0 */
        newdp[0] = dp[0];  /* only value 0 can be appended */

        /* k = 1 … 15 */
        for (idx_t k = 1; k <= 15; ++k) {
            newdp[k] = dp[k] * (seq_t)(k + 1) + dp[k - 1];
        }

        /* copy newdp back to dp for next iteration */
        for (idx_t k = 0; k <= 15; ++k)
            dp[k] = newdp[k];
    }

    /* Sum all dp[k] to get a(n) */
    seq_t total = 0;
    for (idx_t k = 0; k <= 15; ++k)
        total += dp[k];

    return total;
}
