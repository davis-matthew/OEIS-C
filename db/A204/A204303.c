/*
 * A204303  Number of (n+1)X2 0..3 arrays with the sums of 2X2 subblocks
 *          nondecreasing rightwards and downwards.
 * https://oeis.org/A204303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 24, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A204303
 *  Count of (n+1)×2 arrays with entries 0..3 such that the sums of
 *  consecutive 2×2 subblocks are nondecreasing from top to bottom.
 *  (There is only one subblock per pair of rows, so the horizontal
 *  requirement is trivial.)
 *  The algorithm is an O(n) dynamic programming over the
 *  16 possible row pairs and the 13 possible subblock sums (0..12).
 *  It works for any build that supplies integer types seq_t and idx_t.
 */

static const unsigned char sumRow[16] = {
    0,1,2,3,  /* (0,0),(0,1),(0,2),(0,3) */
    1,2,3,4,  /* (1,0),(1,1),(1,2),(1,3) */
    2,3,4,5,  /* (2,0),(2,1),(2,2),(2,3) */
    3,4,5,6   /* (3,0),(3,1),(3,2),(3,3) */
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1, return 0 */

    seq_t dp_prev[16][13] = {0};
    seq_t dp_next[16][13] = {0};

    /* First subblock (row 0 and row 1) – any pair of rows is allowed  */
    for (int p = 0; p < 16; ++p) {
        for (int c = 0; c < 16; ++c) {
            int s = sumRow[p] + sumRow[c];   /* 0..12 */
            dp_next[c][s] += 1;
        }
    }

    if (n == 1) {                 /* only one subblock processed */
        seq_t ans = 0;
        for (int r = 0; r < 16; ++r)
            for (int s = 0; s < 13; ++s)
                ans += dp_next[r][s];
        return ans;
    }

    /* Prepare for further subblocks */
    for (int r = 0; r < 16; ++r)
        for (int s = 0; s < 13; ++s) {
            dp_prev[r][s] = dp_next[r][s];
            dp_next[r][s] = 0;
        }

    /* Process subblocks 2 .. n */
    for (idx_t step = 2; step <= n; ++step) {
        for (int p = 0; p < 16; ++p) {
            for (int s_prev = 0; s_prev < 13; ++s_prev) {
                seq_t ways = dp_prev[p][s_prev];
                if (ways == 0) continue;
                for (int c = 0; c < 16; ++c) {
                    int s = sumRow[p] + sumRow[c];
                    if (s >= s_prev)
                        dp_next[c][s] += ways;
                }
            }
        }
        /* Move current results to dp_prev and clear dp_next */
        for (int r = 0; r < 16; ++r)
            for (int s = 0; s < 13; ++s) {
                dp_prev[r][s] = dp_next[r][s];
                dp_next[r][s] = 0;
            }
    }

    /* Final answer: sum over all states after n subblocks */
    seq_t ans = 0;
    for (int r = 0; r < 16; ++r)
        for (int s = 0; s < 13; ++s)
            ans += dp_prev[r][s];
    return ans;
}
