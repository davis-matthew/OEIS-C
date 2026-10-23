/*
 * A181211  Number of nX7 binary matrices with no two 1's adjacent diagonally
 *          or antidiagonally.
 * https://oeis.org/A181211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary matrices with no two 1’s adjacent
 *            diagonally or antidiagonally. Computed by dynamic programming
 *            over 7‑bit row patterns
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* not used by OEIS, but harmless */

    /* DP arrays: dp[mask] = number of ways ending with this 7‑bit mask. */
    seq_t dp[128];
    seq_t next[128];

    /* Any 7‑bit pattern may be the first row. */
    for (int m = 0; m < 128; ++m)
        dp[m] = (seq_t)1;

    /* Add rows one by one. */
    for (idx_t step = 2; step <= n; ++step)
    {
        /* clear next state */
        for (int m = 0; m < 128; ++m)
            next[m] = (seq_t)0;

        /* transition */
        for (int a = 0; a < 128; ++a)
        {
            seq_t val = dp[a];
            if (val == (seq_t)0) continue;

            for (int b = 0; b < 128; ++b)
            {
                /* no diagonal (a & (b << 1)) and no anti‑diagonal
                   (a & (b >> 1)) adjacency allowed. */
                if ( (a & (b << 1)) == 0 && (a & (b >> 1)) == 0 )
                    next[b] += val;
            }
        }

        /* move to next row */
        for (int m = 0; m < 128; ++m)
            dp[m] = next[m];
    }

    /* total number of matrices of size n×7 */
    seq_t result = (seq_t)0;
    for (int m = 0; m < 128; ++m)
        result += dp[m];

    return result;
}
