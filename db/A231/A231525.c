/*
 * A231525  Number of 3Xn 0..1 arrays with no element less than a strict
 *          majority of its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A231525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sequence A231525 – Number of 3×n binary arrays with no element
 *            less than a strict majority of its horizontal, diagonal and
 *            anti‑diagonal neighbors. The algorithm uses dynamic programming
 *            over columns
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   first_ok_col, last_ok_col, mid_ok_col, neighbor_check
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helper functions below are already supplied by the build environment. */
#include <stdbool.h>
#include "../utils/first_ok_col.h"
#include "../utils/last_ok_col.h"
#include "../utils/mid_ok_col.h"
#include "../utils/neighbor_check.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of offset range – undefined */
    if (n == 1) return 8;            /* all 2³ possible first columns */

    /* --- precompute validity tables for 8 possible columns (0..7) --- */
    enum { COLS = 8 };
    static bool tbl_first[COLS][COLS];
    static bool tbl_last [COLS][COLS];
    static bool tbl_mid  [COLS][COLS][COLS];
    static bool tables_initialized = false;

    if (!tables_initialized) {
        for (int c1 = 0; c1 < COLS; ++c1)
            for (int c2 = 0; c2 < COLS; ++c2) {
                tbl_first[c1][c2] = first_ok_col(c1, c2);
                tbl_last [c1][c2] = last_ok_col(c1, c2);
                for (int c3 = 0; c3 < COLS; ++c3)
                    tbl_mid[c1][c2][c3] = mid_ok_col(c1, c2, c3);
            }
        tables_initialized = true;
    }

    /* DP arrays: dp[prev][curr] counts the ways to reach the current last
       two columns, where 'curr' is the rightmost column.  Only 8×8 entries
       are needed. */
    seq_t dp[COLS][COLS]     = {0};
    seq_t next_dp[COLS][COLS] = {0};

    /* Initialise with first two columns. */
    for (int c1 = 0; c1 < COLS; ++c1)
        for (int c2 = 0; c2 < COLS; ++c2)
            if (tbl_first[c1][c2])
                dp[c1][c2] = 1;

    /* If the matrix has only two columns, perform last-column validation. */
    if (n == 2) {
        seq_t ans = 0;
        for (int prev = 0; prev < COLS; ++prev)
            for (int curr = 0; curr < COLS; ++curr)
                if (tbl_last[prev][curr])
                    ans += dp[prev][curr];
        return ans;
    }

    /* For n ≥ 3: iterate adding columns one by one. */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < COLS; ++i)
            for (int j = 0; j < COLS; ++j)
                next_dp[i][j] = 0;

        for (int prevprev = 0; prevprev < COLS; ++prevprev)
            for (int prev = 0; prev < COLS; ++prev) {
                seq_t cur = dp[prevprev][prev];
                if (cur == 0) continue;
                for (int next = 0; next < COLS; ++next)
                    if (tbl_mid[prevprev][prev][next])
                        next_dp[prev][next] += cur;
            }

        /* Move next_dp to dp for the next iteration. */
        for (int i = 0; i < COLS; ++i)
            for (int j = 0; j < COLS; ++j)
                dp[i][j] = next_dp[i][j];
    }

    /* Final validation of the last column. */
    seq_t ans = 0;
    for (int prev = 0; prev < COLS; ++prev)
        for (int curr = 0; curr < COLS; ++curr)
            if (tbl_last[prev][curr])
                ans += dp[prev][curr];

    return ans;
}
