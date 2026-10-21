/*
 * A073989  Number of strings of length n over Z_6 with trace 3 and subtrace
 *          0.
 * https://oeis.org/A073989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the number of strings of length n over Z_6 whose trace
 *            is 3 and subtrace is 0. Let DP[n][t][s] be the number of strings
 *            of length n with trace t (mod 6) and subtrace s (mod 6). Then
 *            DP[0][0][0] = 1, DP[0][t][s] = 0 for (t,s) ≠ (0,0), DP[n+1][t']
 *            [s'] = Σ₍x=0..5₎ DP[n][t][s] where t' = (t...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* dp holds counts for the current length.
       newdp will hold counts for the next length. */
    seq_t dp[6][6] = {0};
    dp[0][0] = 1;                      /* length 0 */

    for (idx_t len = 0; len < n; ++len) {
        seq_t newdp[6][6];
        /* zero newdp */
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                newdp[i][j] = 0;

        for (int t = 0; t < 6; ++t) {
            for (int s = 0; s < 6; ++s) {
                seq_t val = dp[t][s];
                if (val == 0) continue;      /* skip empty states */
                for (int x = 0; x < 6; ++x) {
                    int t2 = (t + x) % 6;            /* new trace   */
                    int s2 = (s + x * t) % 6;        /* new subtrace */
                    newdp[t2][s2] += val;
                }
            }
        }

        /* copy newdp into dp for the next iteration */
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                dp[i][j] = newdp[i][j];
    }

    return dp[3][0];
}
