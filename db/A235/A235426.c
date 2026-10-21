/*
 * A235426  Number of (n+1)X(1+1) 0..3 arrays with the difference between each
 *          2X2 subblock maximum and minimum lexicographically nondecreasing
 *          rowwise and columnwise.
 * https://oeis.org/A235426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×2 arrays of digits 0..3 such that the
 *            difference (max‑min) of consecutive 2×2 subblocks is
 *            lexicographically nondecreasing rowwise. Computed by dynamic
 *            programming on row patterns
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 16 possible row patterns (left,right) with digits 0..3 */
    static unsigned char diff_tbl[16][16];
    static bool diff_init = false;
    if (!diff_init) {
        for (idx_t i = 0; i < 16; ++i) {
            /* decode pattern i into two cells: left, right */
            idx_t left1 =  i        & 3;
            idx_t right1 = (i >> 2) & 3;
            for (idx_t j = 0; j < 16; ++j) {
                idx_t left2 =  j        & 3;
                idx_t right2 = (j >> 2) & 3;

                idx_t minv = left1;
                if (right1 < minv) minv = right1;
                if (left2  < minv) minv = left2;
                if (right2 < minv) minv = right2;

                idx_t maxv = left1;
                if (right1 > maxv) maxv = right1;
                if (left2  > maxv) maxv = left2;
                if (right2 > maxv) maxv = right2;

                diff_tbl[i][j] = (unsigned char)(maxv - minv);   /* 0..3 */
            }
        }
        diff_init = true;
    }

    /* DP tables: dp[row_pattern][prev_diff] */
    static seq_t dp[16][4];
    static seq_t ndp[16][4];

    /* initialise dp for the first (top) row */
    for (idx_t i = 0; i < 16; ++i)
        for (idx_t d = 0; d < 4; ++d)
            dp[i][d] = (seq_t)0;
    for (idx_t i = 0; i < 16; ++i)
        dp[i][0] = (seq_t)1;   /* no previous difference */

    /* n transitions (from row 0 to row n) */
    for (idx_t step = 1; step <= n; ++step) {
        /* clear ndp */
        for (idx_t i = 0; i < 16; ++i)
            for (idx_t d = 0; d < 4; ++d)
                ndp[i][d] = (seq_t)0;

        if (step == 1) {
            /* first transition: no previous difference constraint */
            for (idx_t i = 0; i < 16; ++i) {
                seq_t cnt = dp[i][0];
                if (!cnt) continue;
                for (idx_t j = 0; j < 16; ++j) {
                    idx_t d = diff_tbl[i][j];
                    ndp[j][d] += cnt;
                }
            }
        } else {
            /* subsequent transitions: enforce nondecreasing diffs */
            for (idx_t i = 0; i < 16; ++i) {
                for (idx_t prev = 0; prev < 4; ++prev) {
                    seq_t cnt = dp[i][prev];
                    if (!cnt) continue;
                    for (idx_t j = 0; j < 16; ++j) {
                        idx_t d = diff_tbl[i][j];
                        if (d >= prev)
                            ndp[j][d] += cnt;
                    }
                }
            }
        }

        /* swap dp and ndp */
        for (idx_t i = 0; i < 16; ++i)
            for (idx_t d = 0; d < 4; ++d)
                dp[i][d] = ndp[i][d];
    }

    /* sum all terminal states */
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < 16; ++i)
        for (idx_t d = 0; d < 4; ++d)
            result += dp[i][d];

    return result;
}
