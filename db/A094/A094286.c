/*
 * A094286  Number of (s(0), s(1), ..., s(n)) such that 0 < s(i) < 6 and |s(i)
 *          - s(i-1)| <= 1 for i = 1,2,...,n, s(0) = 1, s(n) = 1.
 * https://oeis.org/A094286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = |{(s(0),…,s(n)) : 0<s(i)<6, |s(i)-s(i-1)|≤1,
 *            s(0)=s(n)=1}|
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming on a 5‑node path graph. */
seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0u) {
        return (seq_t)1;
    }

    /* dp[h] = number of sequences of current length ending at value h (1..5) */
    seq_t dp[6];
    for (int i = 0; i < 6; ++i) {
        dp[i] = (seq_t)0;
    }
    dp[1] = (seq_t)1; /* start state */

    for (idx_t step = 1; step <= n; ++step) {
        seq_t ndp[6];
        for (int i = 0; i < 6; ++i) {
            ndp[i] = (seq_t)0;
        }
        for (int h = 1; h <= 5; ++h) {
            seq_t sum = dp[h];
            if (h > 1) sum += dp[h - 1];
            if (h < 5) sum += dp[h + 1];
            ndp[h] = sum;
        }
        /* copy new values into dp for next iteration */
        for (int h = 1; h <= 5; ++h) {
            dp[h] = ndp[h];
        }
    }
    return dp[1]; /* must end at 1 */
}
