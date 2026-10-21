/*
 * A073947  Number of strings over Z_3 of length n with trace 0 and subtrace
 *          0.
 * https://oeis.org/A073947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of strings over Z_3 of length n with trace 0 and
 *            subtrace 0. Computed by dynamic programming on the pair (trace,
 *            subtrace) using the recurrence a(n; t, s) = a(n‑1; t, s) +
 *            a(n‑1; t+2, s+2t+1) + a(n‑1; t+1, s+t+1) (mod 3) where t, s ∈
 *            {0,1,2}. Initial state: a(0;0,0)=1, others 0
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* dp and dp_next hold the counts for all 3×3 possible (t,s) pairs. */
    seq_t dp[3][3];
    seq_t dp_next[3][3];

    /* initialise for length 0: only the empty string has t=0,s=0 */
    for (int t = 0; t < 3; ++t)
        for (int s = 0; s < 3; ++s)
            dp[t][s] = 0;
    dp[0][0] = 1;

    /* iterate over the string length */
    for (idx_t len = 1; len <= n; ++len) {
        for (int t = 0; t < 3; ++t) {
            for (int s = 0; s < 3; ++s) {
                /* contributions from adding 0, 1, and 2 respectively */
                seq_t a1 = dp[t][s];
                seq_t a2 = dp[(t + 2) % 3][(s + 2 * t + 1) % 3];
                seq_t a3 = dp[(t + 1) % 3][(s + t + 1) % 3];
                dp_next[t][s] = a1 + a2 + a3;
            }
        }
        /* copy next state back to current */
        for (int t = 0; t < 3; ++t)
            for (int s = 0; s < 3; ++s)
                dp[t][s] = dp_next[t][s];
    }
    /* result is the number of strings with trace 0 and subtrace 0 */
    return dp[0][0];
}
