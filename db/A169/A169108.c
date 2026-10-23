/*
 * A169108  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts reduced words of length n in the Coxeter group with
 *            47 generators and relations S_i^2 = (S_i S_j)^25 = I. The
 *            recurrence arises from the fact that a reduced word has no
 *            adjacent equal letters and never contains an alternating pattern
 *            of two distinct letters of length 25. Let dp[L] den...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)47;
    if (n == 2) return (seq_t)47 * 46;  /* 2162 */

    /* dp[L] for 2 ≤ L ≤ 24 */
    static seq_t dp[25];
    static seq_t dp_next[25];
    for (int i = 0; i < 25; ++i) dp[i] = 0;
    dp[2] = (seq_t)47 * 46; /* 2162 */

    for (idx_t len = 2; len < n; ++len) {
        for (int i = 0; i < 25; ++i) dp_next[i] = 0;
        for (int L = 2; L <= 24; ++L) {
            seq_t val = dp[L];
            if (!val) continue;
            if (L < 24)
                dp_next[L + 1] += val;          /* continue alternation */
            dp_next[2] += val * (seq_t)45;      /* break alternation */
        }
        for (int i = 0; i < 25; ++i) dp[i] = dp_next[i];
    }

    seq_t sum = 0;
    for (int L = 2; L <= 24; ++L) sum += dp[L];
    return sum;
}
