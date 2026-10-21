/*
 * A068787  Number of strings over Z_4 of length n with trace 1 and subtrace
 *          2.
 * https://oeis.org/A068787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of length‑n strings over Z₄ with trace 1 and
 *            subtrace 2. Computed by dynamic programming: for each new digit
 *            d, transition (t , s) → ( (t+d) mod 4 , (s + t·d) mod 4 )
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n ≤ 1 the answer is 0. */
    if (n <= 1)
        return 0;

    /* DP tables for trace and subtrace (mod 4). */
    seq_t dp[4][4]   = {0};
    seq_t newdp[4][4];

    for (idx_t len = 1; len <= n; ++len)
    {
        if (len == 1)
        {
            /* Length 1: each digit produces trace = digit, subtrace = 0. */
            for (int d = 0; d < 4; ++d)
                dp[d][0] += (seq_t)1;
        }
        else
        {
            /* Zero new transition table. */
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    newdp[i][j] = 0;

            /* Apply all possible new digits. */
            for (int t = 0; t < 4; ++t)
            {
                for (int s = 0; s < 4; ++s)
                {
                    seq_t cnt = dp[t][s];
                    if (!cnt) continue;

                    for (int d = 0; d < 4; ++d)
                    {
                        int t2 = (t + d) & 3;           /* (t+d) mod 4 */
                        int s2 = (s + (t * d)) & 3;     /* (s + t*d) mod 4 */
                        newdp[t2][s2] += cnt;
                    }
                }
            }

            /* Copy new table back into dp for next iteration. */
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    dp[i][j] = newdp[i][j];
        }
    }

    return dp[1][2];
}
