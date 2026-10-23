/*
 * A126523  Number of base 28 n-digit numbers with adjacent digits differing
 *          by four or less.
 * https://oeis.org/A126523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{d=0}^{27} dp_n[d] * where dp_1[d] = 1 for all d, *
 *            dp_k[d] = sum_{j:|j-d|≤4} dp_{k-1}[j]. * * Initial values:
 *            a(0)=1, a(1)=28
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0) return (seq_t)1;

    /* Two arrays for current and next DP values */
    seq_t dp[2][28];
    idx_t i;

    /* Initialize dp[0][*] = 1 (all digits allowed for 1-digit numbers) */
    for (i = 0; i < 28; ++i) {
        dp[0][i] = (seq_t)1;
        dp[1][i] = (seq_t)0;          /* will be overwritten in the loop */
    }

    /* If n == 1 we can return the sum directly */
    if (n == 1) {
        seq_t ans = (seq_t)0;
        for (i = 0; i < 28; ++i) ans += dp[0][i];
        return ans;
    }

    int cur = 0, nxt = 1;           /* indexes of the current/next DP row */

    /* DP for lengths 2 .. n */
    for (idx_t len = 2; len <= n; ++len) {
        /* Prefix sums of current DP row to enable O(1) window sums */
        seq_t pref[28];
        pref[0] = dp[cur][0];
        for (i = 1; i < 28; ++i) pref[i] = pref[i-1] + dp[cur][i];

        /* Compute next DP row */
        for (i = 0; i < 28; ++i) {
            idx_t lo = (i > 4) ? i - 4 : 0;
            idx_t hi = (i + 4 < 28) ? i + 4 : 27;
            seq_t val = pref[hi];
            if (lo > 0) val -= pref[lo-1];
            dp[nxt][i] = val;
        }

        /* Swap current and next rows */
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /* Sum all DP values of the final row to get a(n) */
    seq_t ans = (seq_t)0;
    for (i = 0; i < 28; ++i) ans += dp[cur][i];
    return ans;
}
