/*
 * A207113  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 50, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A207113:
   a(n) = number of 4 × n 0..1 arrays avoiding the patterns
   horizontally 000 and 011, and vertically 001 and 110.
   The sequence is computed by dynamic programming over column
   masks of 4 bits, respecting the vertical restrictions for each
   column and checking the horizontal restrictions for each
   consecutive triple of columns. */

seq_t a(idx_t n)
{
    /* The number of valid column masks (satisfying vertical constraints) */
    static int v[8];
    static int vcount = 0;
    static int mask_to_index[16];
    static int init_done = 0;

    /* Perform one-time initialization of mask lists */
    if (!init_done) {
        for (int m = 0; m < 16; ++m) {
            /* Extract bits of the 4‑bit mask */
            int b0 = (m >> 0) & 1;
            int b1 = (m >> 1) & 1;
            int b2 = (m >> 2) & 1;
            int b3 = (m >> 3) & 1;
            /* Vertical constraints: no 001 or 110 in rows 1‑3 or 2‑4 */
            if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                (b0 == 1 && b1 == 1 && b2 == 0))
                continue;
            if ((b1 == 0 && b2 == 0 && b3 == 1) ||
                (b1 == 1 && b2 == 1 && b3 == 0))
                continue;
            /* This mask is allowed */
            v[vcount++] = m;
            mask_to_index[m] = vcount - 1;
        }
        for (int m = 0; m < 16; ++m)
            if (mask_to_index[m] == 0)
                mask_to_index[m] = -1;  /* not used, safe */
        init_done = 1;
    }

    /* Handle trivial cases directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)vcount;
    if (n == 2) return (seq_t)vcount * (seq_t)vcount;

    /* DP arrays: dp[prev][cur] holds number of ways for the last
       two columns having masks indices prev and cur.
       Max dimension needed is 8 (since vcount ≤ 8). */
    static seq_t dp[8][8];
    static seq_t nextdp[8][8];

    /* Initialise all entries to zero */
    for (int i = 0; i < vcount; ++i)
        for (int j = 0; j < vcount; ++j)
            dp[i][j] = 0;

    /* After two columns, every pair of allowed masks contributes one
       arrangement. */
    for (int i = 0; i < vcount; ++i)
        for (int j = 0; j < vcount; ++j)
            dp[i][j] = 1;

    /* Transition for columns from 3 to n */
    for (idx_t col = 3; col <= n; ++col) {
        /* Zero nextdp */
        for (int i = 0; i < vcount; ++i)
            for (int j = 0; j < vcount; ++j)
                nextdp[i][j] = 0;

        for (int i = 0; i < vcount; ++i) {
            int prev_mask = v[i];
            for (int j = 0; j < vcount; ++j) {
                seq_t cur_val = dp[i][j];
                if (cur_val == 0) continue;
                int cur_mask = v[j];
                for (int k = 0; k < vcount; ++k) {
                    int next_mask = v[k];
                    /* Check horizontal constraints for all four rows */
                    int ok = 1;
                    for (int r = 0; r < 4 && ok; ++r) {
                        int a = (prev_mask >> r) & 1;
                        int b = (cur_mask >> r) & 1;
                        int c = (next_mask >> r) & 1;
                        if ((a == 0 && b == 0 && c == 0) ||
                            (a == 0 && b == 1 && c == 1))
                            ok = 0;
                    }
                    if (!ok) continue;
                    /* Accumulate */
                    nextdp[j][k] += cur_val;
                }
            }
        }
        /* Prepare for next iteration */
        for (int i = 0; i < vcount; ++i)
            for (int j = 0; j < vcount; ++j)
                dp[i][j] = nextdp[i][j];
    }

    /* Sum all DP entries for the last position to get total count */
    seq_t result = 0;
    for (int i = 0; i < vcount; ++i)
        for (int j = 0; j < vcount; ++j)
            result += dp[i][j];

    return result;
}
