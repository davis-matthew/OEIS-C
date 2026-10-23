/*
 * A189105  Number of n X 3 binary arrays without the pattern 1 1 0 diagonally
 *          or antidiagonally.
 * https://oeis.org/A189105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 binary arrays without the * pattern 1 1 0
 *            diagonally or antidiagonally. * For n = 1 → 8, n = 2 → 64, and
 *            for n ≥ 3 the values are computed * by dynamic programming over
 *            the previous two rows
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)64;

    seq_t dp[64];
    seq_t newdp[64];

    /* initialize: all 2‑row prefixes are allowed, count = 1 each */
    for (int i = 0; i < 64; ++i) dp[i] = (seq_t)1;

    /* number of additional rows to add after the initial two */
    idx_t extra = n - 2;
    for (idx_t step = 0; step < extra; ++step) {
        for (int i = 0; i < 64; ++i) newdp[i] = (seq_t)0;

        for (int idx = 0; idx < 64; ++idx) {
            seq_t count = dp[idx];
            if (!count) continue;

            int s   = idx >> 3;   /* older row (row-2) */
            int i2  = idx & 7;    /* recent row (row-1) */

            for (int r = 0; r < 8; ++r) {
                /* forbidden patterns:
                 * (s[0] = 1, i2[1] = 1, r[2] = 0)   diagonal
                 * (s[2] = 1, i2[1] = 1, r[0] = 0)   antidiagonal */
                int b_s0 = (s >> 0) & 1;
                int b_s2 = (s >> 2) & 1;
                int b_i1 = (i2 >> 1) & 1;
                int b_r2 = (r >> 2) & 1;
                int b_r0 = r & 1;

                if ((b_s0 && b_i1 && !b_r2) || (b_s2 && b_i1 && !b_r0))
                    continue;

                newdp[(i2 << 3) | r] += count;
            }
        }
        /* copy newdp to dp for next iteration */
        for (int i = 0; i < 64; ++i) dp[i] = newdp[i];
    }

    seq_t total = (seq_t)0;
    for (int i = 0; i < 64; ++i) total += dp[i];
    return total;
}
