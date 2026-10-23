/*
 * A126360  Number of base 6 n-digit numbers with adjacent digits differing by
 *          one or less.
 * https://oeis.org/A126360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base 6 n‑digit numbers with adjacent digits
 *            differing by one or less. Recurrence (dynamic programming): for
 *            each length keep counts of strings ending with each digit 0‑5.
 *            Complexity: O(n). Works for any seq_t (integral or
 *            arbitrary‑precision)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 42, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;

    /* dp[d] = number of length‑1 strings ending with digit d */
    seq_t dp[6];
    for (int d = 0; d < 6; ++d)
        dp[d] = (seq_t)1;

    if (n == 1u)
        return (seq_t)6;

    seq_t ndp[6];
    for (idx_t len = 2; len <= n; ++len) {
        for (int d = 0; d < 6; ++d) {
            seq_t s = dp[d];
            if (d > 0) s += dp[d - 1];
            if (d < 5) s += dp[d + 1];
            ndp[d] = s;
        }
        for (int d = 0; d < 6; ++d)
            dp[d] = ndp[d];
    }

    seq_t total = dp[0] + dp[1] + dp[2] + dp[3] + dp[4] + dp[5];
    return total;
}
