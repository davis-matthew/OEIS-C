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
 * Validated: standard, pass, 36 terms through n = 210 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helper functions below are already supplied by the build environment. */
#include <stdbool.h>

/* Check that the value of each cell in the middle column C satisfies the
   rule with its neighbors given by columns L (left) and R (right).
   'hasL' and 'hasR' indicate whether the corresponding neighbor column
   exists.  Zero values are forbidden only if the strict majority of
   neighboring ones exceeds half the available neighbors.  One values are
   never forbidden, since they are never less than a strict majority. */
static inline bool neighbor_check(int L, int C, int R,
                                  bool hasL, bool hasR)
{
    for (int r = 0; r < 3; ++r) {
        int neigh = 0;          /* number of existing neighbors */
        int ones  = 0;          /* number of ones among them */

        /* horizontal left and right */
        if (hasL) { neigh++; ones += (L >> r) & 1; }
        if (hasR) { neigh++; ones += (R >> r) & 1; }

        /* diagonals: up-left / up-right / down-left / down-right */
        if (hasL && r > 0)   { neigh++; ones += (L >> (r-1)) & 1; }
        if (hasR && r > 0)   { neigh++; ones += (R >> (r-1)) & 1; }
        if (hasL && r < 2)   { neigh++; ones += (L >> (r+1)) & 1; }
        if (hasR && r < 2)   { neigh++; ones += (R >> (r+1)) & 1; }

        int val = (C >> r) & 1;                 /* value of the cell */

        /* For a zero cell, reject if the strict majority of neighbors
           are ones: that means ones > floor(neigh/2). */
        if (val == 0 && ones > neigh / 2)
            return false;
    }
    return true;
}

/* Column validity predicates for the first, middle and last columns. */
static inline bool first_ok_col(int C, int R)
{
    return neighbor_check(-1, C, R, false, true);
}

static inline bool last_ok_col(int L, int C)
{
    return neighbor_check(L, C, -1, true, false);
}

static inline bool mid_ok_col(int L, int C, int R)
{
    return neighbor_check(L, C, R, true, true);
}

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
