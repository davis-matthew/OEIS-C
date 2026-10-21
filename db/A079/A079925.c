/*
 * A079925  Solution to the Dancing School Problem with n girls and n+6 boys:
 *          f(n,6).
 * https://oeis.org/A079925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = permanent of B, where B_{i,j} = 1 iff i <= j <= i+6 (size
 *            n×(n+6)). Computed via DP over 7‑bit state
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const int H = 6;               /* band width   */
    const int MSIZE = 128;         /* 2^(H+1) bitmasks */

    seq_t dp[MSIZE];
    seq_t ndp[MSIZE];

    /* initialise dp[0] = 1, others 0 */
    for (int i = 0; i < MSIZE; ++i) dp[i] = 0;
    dp[0] = 1;

    for (idx_t row = 0; row < n; ++row)
    {
        /* clear next dp array */
        for (int i = 0; i < MSIZE; ++i) ndp[i] = 0;

        /* transition from all states */
        for (int m = 0; m < MSIZE; ++m)
        {
            seq_t cur = dp[m];
            if (cur == 0) continue;

            /* try to match current row to a free column offset k (0..H) */
            for (int k = 0; k <= H; ++k)
            {
                if ((m & (1 << k)) == 0)
                {
                    int m_next  = m | (1 << k);   /* mark column as used */
                    int m_shift = m_next >> 1;    /* shift for next row, drop column i */
                    ndp[m_shift] += cur;
                }
            }
        }

        /* copy back to dp */
        for (int i = 0; i < MSIZE; ++i) dp[i] = ndp[i];
    }

    /* sum over all ending states */
    seq_t total = 0;
    for (int m = 0; m < MSIZE; ++m) total += dp[m];
    return total;
}
