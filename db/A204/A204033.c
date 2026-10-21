/*
 * A204033  Number of (n+1)X3 0..1 arrays with the sums of 2X2 subblocks
 *          nondecreasing rightwards and downwards.
 * https://oeis.org/A204033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence‑based DP a(n) = number of (n+1)×3 binary arrays whose
 *            2×2 sub‑block sums are nondecreasing rightwards and downwards.
 *            The computation is a straightforward dynamic programme over the
 *            8 possible rows
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 22, u64 n <= 54, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; safe guard */

    /* All possible rows (3 bits). */
    static const int bits[8][3] = {
        {0,0,0}, {1,0,0}, {0,1,0}, {1,1,0},
        {0,0,1}, {1,0,1}, {0,1,1}, {1,1,1}
    };

    /* Pre‑compute 2×2 block sums for every pair of rows. */
    static int sum01[8][8];
    static int sum12[8][8];
    static int init_done = 0;
    if (!init_done) {
        for (int p = 0; p < 8; ++p) {
            for (int q = 0; q < 8; ++q) {
                sum01[p][q] = bits[p][0] + bits[p][1] + bits[q][0] + bits[q][1];
                sum12[p][q] = bits[p][1] + bits[p][2] + bits[q][1] + bits[q][2];
            }
        }
        init_done = 1;
    }

    /* dp_prev[p] = 1 represents the 1st row before any 2×2 blocks. */
    static seq_t dp_prev[8];
    for (int i = 0; i < 8; ++i) dp_prev[i] = 1;

    /* dp_curr[last][s0][s1] – counts after processing a row.
       s0,s1 ∈ [0,4] (max 2×2 sum). */
    static seq_t dp_curr[8][5][5];
    static seq_t dp_next[8][5][5];

    /* Clear dp_curr for the first iteration. */
    for (int a = 0; a < 8; ++a)
        for (int b = 0; b < 5; ++b)
            for (int c = 0; c < 5; ++c)
                dp_curr[a][b][c] = 0;

    /* There are (n+1) rows.  We start at row 2 (the second row). */
    for (idx_t row = 2; row <= n + 1; ++row) {

        /* Clear dp_next if needed. */
        if (row > 2) {
            for (int a = 0; a < 8; ++a)
                for (int b = 0; b < 5; ++b)
                    for (int c = 0; c < 5; ++c)
                        dp_next[a][b][c] = 0;
        }

        if (row == 2) {
            /* Transition from the first row to the second row.
               Only horizontal monotonicity applies: s0 <= s1. */
            for (int p = 0; p < 8; ++p)
                for (int q = 0; q < 8; ++q) {
                    seq_t cnt = dp_prev[p];
                    if (!cnt) continue;
                    int s0 = sum01[p][q];
                    int s1 = sum12[p][q];
                    if (s0 > s1) continue;
                    dp_curr[q][s0][s1] += cnt;
                }
        } else {
            /* Transition from a state with previous 2×2 sums to the next row. */
            for (int last = 0; last < 8; ++last)
                for (int s0prev = 0; s0prev < 5; ++s0prev)
                    for (int s1prev = 0; s1prev < 5; ++s1prev) {
                        seq_t cnt = dp_curr[last][s0prev][s1prev];
                        if (!cnt) continue;
                        for (int q = 0; q < 8; ++q) {
                            int s0 = sum01[last][q];
                            int s1 = sum12[last][q];
                            if (s0 > s1) continue;          /* horizontal */
                            if (s0prev > s0) continue;      /* vertical (col1–col2) */
                            if (s1prev > s1) continue;      /* vertical (col2–col3) */
                            dp_next[q][s0][s1] += cnt;
                        }
                    }
            /* Move dp_next into dp_curr for the next iteration. */
            for (int a = 0; a < 8; ++a)
                for (int b = 0; b < 5; ++b)
                    for (int c = 0; c < 5; ++c)
                        dp_curr[a][b][c] = dp_next[a][b][c];
        }
    }

    /* Sum all terminal states to get the total number of arrays. */
    seq_t result = 0;
    for (int last = 0; last < 8; ++last)
        for (int s0 = 0; s0 < 5; ++s0)
            for (int s1 = 0; s1 < 5; ++s1)
                result += dp_curr[last][s0][s1];

    return result;
}
