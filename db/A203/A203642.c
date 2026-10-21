/*
 * A203642  Number of arrays of n 0..11 integers with new values introduced in
 *          order 0..11 but otherwise unconstrained.
 * https://oeis.org/A203642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1}^{12} S(n,k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t dp[13];
    dp[0] = (seq_t)1;
    for (int k = 1; k <= 12; ++k) dp[k] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        for (int k = 12; k >= 1; --k) {
            dp[k] = dp[k] * (seq_t)k + dp[k-1];
        }
        dp[0] = 0;
    }
    seq_t sum = 0;
    for (int k = 1; k <= 12; ++k) sum += dp[k];
    return sum;
}
