/*
 * A068778  S(n; 1,1) = S(n; 3,1) where S(n; t,s) is the number of length n
 *          4-ary strings whose digits sum to t mod 4 and whose sum of
 *          products of all pairs of digits sum to s mod 4.
 * https://oeis.org/A068778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   S(n; t, s) = S(n-1; t, s)
              + S(n-1; (t+3) mod 4, (s+3t+1) mod 4)
              + S(n-1; (t+2) mod 4, (s+2t) mod 4)
              + S(n-1; (t+1) mod 4, (s+t+1) mod 4)
   a(n) = S(n; 1,1) = S(n; 3,1)
*/
seq_t a(idx_t n) {
    seq_t dp[2][4][4];
    /* initialize for n = 0: only one empty string -> sum 0, product sum 0 */
    for (int t = 0; t < 4; ++t)
        for (int s = 0; s < 4; ++s)
            dp[0][t][s] = 0;
    dp[0][0][0] = 1;

    int cur = 0, nxt = 1;
    for (idx_t i = 1; i <= n; ++i) {
        for (int t = 0; t < 4; ++t) {
            for (int s = 0; s < 4; ++s) {
                seq_t val = dp[cur][t][s]
                          + dp[cur][(t + 3) & 3][(s + 3 * t + 1) & 3]
                          + dp[cur][(t + 2) & 3][(s + 2 * t) & 3]
                          + dp[cur][(t + 1) & 3][(s + t + 1) & 3];
                dp[nxt][t][s] = val;
            }
        }
        int tmp = cur; cur = nxt; nxt = tmp;  /* swap current and next */
    }
    return dp[cur][1][1];
}
