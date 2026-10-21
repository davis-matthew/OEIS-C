/*
 * A205208  Number of (n+1) X 2 0..2 arrays with the determinants of 2 X 2
 *          subblocks nondecreasing rightwards and downwards.
 * https://oeis.org/A205208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS A205208:
   Number of (n+1)×2 arrays with entries 0..2 whose 2×2 determinants
   are nondecreasing downwards. The algorithm is a direct DP over rows:
   A 2×2 determinant of rows r and s is a0*b1 - a1*b0 where
   r = (a0,a1), s = (b0,b1).  The state is (last row, previous
   determinant); there are at most 9 rows and 9 possible determinant
   values (-4..4), so the DP is tiny and runs in O(n). It does not
   rely on hard‑coded initial values. */

#include <stdint.h>

static const unsigned char ROWS[9][2] = {
    {0,0}, {0,1}, {0,2},
    {1,0}, {1,1}, {1,2},
    {2,0}, {2,1}, {2,2}
};

/* Compute determinant of two rows indexed r0 and r1 */
static inline int
det_idx(int r0, int r1)
{
    return (int)ROWS[r0][0] * ROWS[r1][1]
         - (int)ROWS[r0][1] * ROWS[r1][0];
}

/* a(n) for n >= 1. For n < 1 the function returns 0. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* DP tables: dp[last_row][det_index] */
    static seq_t dp[9][9];
    static seq_t newdp[9][9];

    /* initialise dp to zero */
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            dp[i][j] = 0;

    /* first pair of rows */
    for (int r0 = 0; r0 < 9; ++r0) {
        for (int r1 = 0; r1 < 9; ++r1) {
            int d = det_idx(r0, r1);   /* -4 .. 4 */
            int idx = d + 4;           /* 0 .. 8 */
            dp[r1][idx] += (seq_t)1;
        }
    }

    /* if only two rows (n==1) we are done */
    if (n == 1) {
        seq_t result = 0;
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                result += dp[i][j];
        return result;
    }

    /* add rows 2..n (total n-1 transitions) */
    for (idx_t step = 2; step <= n; ++step) {
        /* zero newdp */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                newdp[i][j] = 0;

        /* transition */
        for (int last = 0; last < 9; ++last) {
            for (int dprev_idx = 0; dprev_idx < 9; ++dprev_idx) {
                seq_t cnt = dp[last][dprev_idx];
                if (cnt == 0) continue;
                int dprev = dprev_idx - 4;
                for (int next = 0; next < 9; ++next) {
                    int dnew = det_idx(last, next);
                    if (dnew >= dprev) {
                        int idxnew = dnew + 4;
                        newdp[next][idxnew] += cnt;
                    }
                }
            }
        }

        /* copy newdp into dp for next iteration */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* sum all states to get the total count */
    seq_t result = 0;
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            result += dp[i][j];

    return result;
}
