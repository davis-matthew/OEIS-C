/*
 * A011561  Stirling numbers of second kind S2(12,n).
 * https://oeis.org/A011561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = S2(12, n) – Stirling number of the second kind for 12 elements.
 * The values satisfy:  S(k, j) = j * S(k-1, j) + S(k-1, j-1) with
 * S(0, 0) = 1 and S(k, 0) = 0 for k > 0.
 * a(n) is zero for n < 1 or n > 12.
 */
seq_t a(idx_t n)
{
    /* static array holding S2(12, k) for k = 0..12 */
    static seq_t s12[13];
    static int ready = 0;

    if (!ready)
    {
        /* compute the whole triangle up to n = 12 */
        static seq_t dp[13][13];
        dp[0][0] = 1;
        for (idx_t i = 1; i <= 12; ++i)
        {
            dp[i][0] = 0;
            for (idx_t j = 1; j <= i; ++j)
            {
                dp[i][j] = (seq_t)j * dp[i - 1][j] + dp[i - 1][j - 1];
            }
        }
        for (idx_t j = 0; j <= 12; ++j)
            s12[j] = dp[12][j];
        ready = 1;
    }

    if (n < 1 || n > 12)
        return (seq_t)0;
    return s12[n];
}
