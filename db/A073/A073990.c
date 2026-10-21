/*
 * A073990  Number of strings of length n over Z_6 with trace 3 and subtrace
 *          1.
 * https://oeis.org/A073990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the number of strings of length n over Z₆ whose trace
 * (sum of digits modulo 6) equals 3 and subtrace (sum of products of all
 * unordered pairs of distinct digits modulo 6) equals 1.
 * The DP below computes a(n) for any n ≥ 1 using a 6×6 table of states.
 */
seq_t a(idx_t n)
{
    /* DP tables for (trace, subtrace) states modulo 6 */
    seq_t dp[6][6] = {0};
    seq_t nx[6][6] = {0};

    /* empty string */
    dp[0][0] = 1;

    /* iterate over string length */
    for (idx_t step = 1; step <= n; ++step)
    {
        /* clear next table */
        for (int t = 0; t < 6; ++t)
            for (int s = 0; s < 6; ++s)
                nx[t][s] = 0;

        /* transition */
        for (int t = 0; t < 6; ++t)
        {
            for (int s = 0; s < 6; ++s)
            {
                seq_t val = dp[t][s];
                if (val == 0) continue;
                for (int d = 0; d < 6; ++d)
                {
                    int t2 = (t + d) % 6;
                    /* subtrace increment: d * t (mod 6) */
                    int inc = (d * t) % 6;
                    int s2 = (s + inc) % 6;
                    nx[t2][s2] = nx[t2][s2] + val;
                }
            }
        }

        /* copy back */
        for (int t = 0; t < 6; ++t)
            for (int s = 0; s < 6; ++s)
                dp[t][s] = nx[t][s];
    }

    /* result for trace 3 and subtrace 1 */
    return dp[3][1];
}
