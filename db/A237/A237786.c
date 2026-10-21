/*
 * A237786  Number of (n+1)X(1+1) 0..3 arrays with the maximum plus the
 *          minimum minus the lower median of every 2X2 subblock differing
 *          from its horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A237786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×2 arrays over {0,1,2,3} where for every
 *            2×2 subblock the value (max + min – second_smallest) of the
 *            subblock differs from that of its directly adjacent vertical
 *            subblock by exactly one. Computation is performed by dynamic
 *            programming over the sequence of rows
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   calc_f
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/calc_f.h"

static const uint8_t ROWS[16][2] = {
    {0,0},{0,1},{0,2},{0,3},
    {1,0},{1,1},{1,2},{1,3},
    {2,0},{2,1},{2,2},{2,3},
    {3,0},{3,1},{3,2},{3,3}
};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Precompute the f-values for all row pairs once. */
    static uint8_t ftable[16][16];
    static int ftable_done = 0;
    if (!ftable_done) {
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 16; ++j) {
                ftable[i][j] = calc_f(ROWS[i][0], ROWS[i][1],
                                      ROWS[j][0], ROWS[j][1]);
            }
        }
        ftable_done = 1;
    }

    /* Dynamic programming arrays: 16 rows × 4 f-values. */
    seq_t dp_cur[16][4];
    seq_t dp_next[16][4];
    for (int r = 0; r < 16; ++r)
        for (int f = 0; f < 4; ++f)
            dp_cur[r][f] = (seq_t)0;

    /* Initial counting for the first subblock (rows 0 and 1). */
    for (int r0 = 0; r0 < 16; ++r0) {
        for (int r1 = 0; r1 < 16; ++r1) {
            uint8_t f = ftable[r0][r1];
            dp_cur[r1][f] += (seq_t)1;
        }
    }

    /* Transition for the remaining subblocks (n-1 steps). */
    for (idx_t step = 1; step < n; ++step) {
        for (int r = 0; r < 16; ++r)
            for (int f = 0; f < 4; ++f)
                dp_next[r][f] = (seq_t)0;

        for (int ri = 0; ri < 16; ++ri) {
            for (int fprev = 0; fprev < 4; ++fprev) {
                const seq_t val = dp_cur[ri][fprev];
                if (val == (seq_t)0) continue;
                for (int rj = 0; rj < 16; ++rj) {
                    uint8_t fnext = ftable[ri][rj];
                    uint8_t diff = (fprev > fnext) ? fprev - fnext : fnext - fprev;
                    if (diff == 1) {
                        dp_next[rj][fnext] += val;
                    }
                }
            }
        }

        /* Copy dp_next into dp_cur for the next iteration. */
        for (int r = 0; r < 16; ++r)
            for (int f = 0; f < 4; ++f)
                dp_cur[r][f] = dp_next[r][f];
    }

    /* Sum all final states to obtain the answer. */
    seq_t ans = (seq_t)0;
    for (int r = 0; r < 16; ++r)
        for (int f = 0; f < 4; ++f)
            ans += dp_cur[r][f];

    return ans;
}
