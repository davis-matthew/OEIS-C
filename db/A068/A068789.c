/*
 * A068789  S(n; 2,0) where S(n; t,s) is the number of length n 4-ary strings
 *          whose digits sum to t mod 4 and whose sum of products of all pairs
 *          of digits sum to s mod 4.
 * https://oeis.org/A068789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = S(n; 2, 0) where S satisfies S(n; t, s) = S(n-1; t, s) +
 *            S(n-1; (t+3)&3, (s+3*t+1)&3) + S(n-1; (t+2)&3, (s+2*t)&3) +
 *            S(n-1; (t+1)&3, (s+t+1)&3) with base S(1; t, 0) = 1 and S(1; t,
 *            s≠0) = 0
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* safety: counts start at n=1 */
    seq_t dp[4][4];
    seq_t ndp[4][4];

    /* initialisation for n = 1 */
    for (int t = 0; t < 4; ++t)
        for (int s = 0; s < 4; ++s) dp[t][s] = 0;
    for (int t = 0; t < 4; ++t) dp[t][0] = 1;

    if (n == 1) return dp[2][0];

    for (idx_t len = 2; len <= n; ++len) {
        /* clear next-state array */
        for (int t = 0; t < 4; ++t)
            for (int s = 0; s < 4; ++s)
                ndp[t][s] = 0;

        for (int t = 0; t < 4; ++t) {
            for (int s = 0; s < 4; ++s) {
                ndp[t][s] = dp[t][s];                 /* d = 0 */
                int t3 = (t + 3) & 3;                  /* d = 1 */
                int s3 = (s + 3 * t + 1) & 3;
                ndp[t][s] += dp[t3][s3];
                int t2 = (t + 2) & 3;                  /* d = 2 */
                int s2 = (s + 2 * t) & 3;
                ndp[t][s] += dp[t2][s2];
                int t1 = (t + 1) & 3;                  /* d = 3 */
                int s1 = (s + t + 1) & 3;
                ndp[t][s] += dp[t1][s1];
            }
        }
        /* copy ndp back to dp for next iteration */
        for (int t = 0; t < 4; ++t)
            for (int s = 0; s < 4; ++s)
                dp[t][s] = ndp[t][s];
    }

    return dp[2][0];
}
