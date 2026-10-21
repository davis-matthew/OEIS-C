/*
 * A203104  Number of nX4 binary arrays with every 1 immediately preceded by 0
 *          0 to the left or above.
 * https://oeis.org/A203104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A203104: Number of n×4 binary arrays where each 1 has two consecutive
   zeros directly to its left in the same row, or directly above in the
   previous two rows.  Computed by dynamic programming over rows and
   4‑bit column patterns.  The DP state is the last two rows' bit patterns. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* offset is 1; n ≥ 1 is required */
        return 0;

    /* dp[state] where state = prev1<<4 | prev2, each 4‑bit pattern. */
    seq_t dp[256];
    seq_t next[256];
    for (int i = 0; i < 256; ++i) dp[i] = 0;
    dp[0] = 1;                     /* empty initial pattern */

    for (idx_t row = 1; row <= n; ++row) {
        for (int i = 0; i < 256; ++i) next[i] = 0;

        for (int prev1 = 0; prev1 < 16; ++prev1) {
            for (int prev2 = 0; prev2 < 16; ++prev2) {
                seq_t cur = dp[(prev1 << 4) | prev2];
                if (!cur) continue;             /* skip zero counts */

                for (int pat = 0; pat < 16; ++pat) {
                    int ok = 1;
                    for (int j = 0; j < 4 && ok; ++j) {
                        if (!(pat & (1 << j))) continue;   /* not a 1 */

                        /* check two zeros to the left        */
                        int left_ok = 0;
                        if (j >= 2) {
                            if (!(pat & (1 << (j - 1))) &&
                                !(pat & (1 << (j - 2))))
                                left_ok = 1;
                        }

                        /* check two zeros directly above      */
                        int vert_ok = 0;
                        if (row >= 3) {
                            if (!(prev1 & (1 << j)) &&
                                !(prev2 & (1 << j)))
                                vert_ok = 1;
                        }

                        if (!(left_ok || vert_ok)) ok = 0;
                    }
                    if (!ok) continue;

                    int new_prev1 = pat;
                    int new_prev2 = prev1;
                    next[(new_prev1 << 4) | new_prev2] += cur;
                }
            }
        }

        /* transfer next→dp for the next row */
        for (int i = 0; i < 256; ++i) dp[i] = next[i];
    }

    /* sum over all terminal states */
    seq_t total = 0;
    for (int i = 0; i < 256; ++i) total += dp[i];
    return total;
}
