/*
 * A055425  Number of points in N^n of norm <= 10.
 * https://oeis.org/A055425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{s=0}^{100} coeff of x^s in (∑_{k=0}^{10} x^{k^2})^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 30, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const int squares[11] = {0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100};
    seq_t dp[101];
    seq_t next[101];

    for (idx_t i = 0; i < 101; ++i) dp[i] = 0;
    dp[0] = 1;

    for (idx_t step = 0; step < n; ++step) {
        for (idx_t i = 0; i < 101; ++i) next[i] = 0;
        for (idx_t s = 0; s < 101; ++s) {
            for (int k = 0; k < 11; ++k) {
                idx_t t = s + squares[k];
                if (t > 100) break;
                next[t] += dp[s];
            }
        }
        for (idx_t i = 0; i < 101; ++i) dp[i] = next[i];
    }

    seq_t res = 0;
    for (idx_t s = 0; s < 101; ++s) res += dp[s];
    return res;
}
