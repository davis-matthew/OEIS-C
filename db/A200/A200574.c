/*
 * A200574  Number of nX3 0..2 arrays with no average of any element and its
 *          horizontal and vertical neighbors equal to one.
 * https://oeis.org/A200574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   valid_cell, valid_row_bottom, valid_row_middle,
 *            valid_row_single, valid_row_top
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A200574: number of n×3 arrays with entries 0..2 in which
   no cell has average 1 over itself and its horizontal and vertical
   neighbours. */

/* The DP keeps the last two rows.  There are 27 possible rows (3^3),
   so the state space is 27×27.  Each transition checks the middle
   row, governed by its neighbours above and below.  The last row is
   checked separately at the end. */

#include <stdbool.h>
#include <stddef.h>    /* for NULL */
#include "../utils/valid_cell.h"
#include "../utils/valid_row_bottom.h"
#include "../utils/valid_row_middle.h"
#include "../utils/valid_row_single.h"
#include "../utils/valid_row_top.h"

/* all possible 3‑column rows over {0,1,2} */
static const unsigned char rows[27][3] = {
    {0,0,0},{0,0,1},{0,0,2},{0,1,0},{0,1,1},{0,1,2},{0,2,0},{0,2,1},{0,2,2},
    {1,0,0},{1,0,1},{1,0,2},{1,1,0},{1,1,1},{1,1,2},{1,2,0},{1,2,1},{1,2,2},
    {2,0,0},{2,0,1},{2,0,2},{2,1,0},{2,1,1},{2,1,2},{2,2,0},{2,2,1},{2,2,2}
};

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n=0, return 0 */

    /* DP tables: dp[prevprev][prev] */
    static seq_t dp[27][27];
    static seq_t ndp[27][27];

    /* initialise to zero */
    for (int i = 0; i < 27; ++i)
        for (int j = 0; j < 27; ++j)
            dp[i][j] = (seq_t)0;

    /* case n==1 */
    if (n == 1) {
        seq_t count = (seq_t)0;
        for (int r = 0; r < 27; ++r)
            if (valid_row_single(rows[r]))
                count += (seq_t)1;
        return count;
    }

    /* initialise first two rows, verifying the top row */
    for (int r0 = 0; r0 < 27; ++r0) {
        for (int r1 = 0; r1 < 27; ++r1) {
            if (valid_row_top(rows[r0], rows[r1]))
                dp[r0][r1] = (seq_t)1;
        }
    }

    /* iterate subsequent rows */
    for (idx_t row = 2; row < n; ++row) {
        /* zero ndp */
        for (int i = 0; i < 27; ++i)
            for (int j = 0; j < 27; ++j)
                ndp[i][j] = (seq_t)0;

        /* transition */
        for (int prevprev = 0; prevprev < 27; ++prevprev) {
            for (int prev = 0; prev < 27; ++prev) {
                seq_t ways = dp[prevprev][prev];
                if (ways == (seq_t)0) continue;
                for (int cur = 0; cur < 27; ++cur) {
                    if (valid_row_middle(rows[prevprev], rows[prev], rows[cur]))
                        ndp[prev][cur] += ways;
                }
            }
        }

        /* copy ndp back to dp */
        for (int i = 0; i < 27; ++i)
            for (int j = 0; j < 27; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* finalize: validate last row (no down neighbour) */
    seq_t total = (seq_t)0;
    for (int prevprev = 0; prevprev < 27; ++prevprev) {
        for (int prev = 0; prev < 27; ++prev) {
            seq_t ways = dp[prevprev][prev];
            if (ways == (seq_t)0) continue;
            if (valid_row_bottom(rows[prevprev], rows[prev]))
                total += ways;
        }
    }
    return total;
}
