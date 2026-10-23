/*
 * A183547  Number of n X 2 0..2 arrays with each element equal to either the
 *          maximum or the minimum of its horizontal and vertical neighbors.
 * https://oeis.org/A183547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes the number of n × 2 arrays with entries in {0,1,2} such that
 * each element equals the maximum or the minimum of its horizontal
 * (left/right in the same row) and vertical (up/down) neighbors.
 * The implementation uses dynamic programming over pairs of consecutive
 * rows and is valid for all n ≥ 1.
 */

#include <stdbool.h>

/* Encode a 2‑cell row as a single integer 0..8:
 *  left value = code/3, right value = code%3. */
static const int row_left[9]  = {0,0,0,1,1,1,2,2,2};
static const int row_right[9] = {0,1,2,0,1,2,0,1,2};

/* Check if row 'r' is valid as the first row with only a below neighbour.
 * 'down' is the row below 'r'. */
static inline bool first_row_ok(int r, int down)
{
    int l   = row_left[r];
    int rr  = row_right[r];
    int d0  = row_left[down];
    int d1  = row_right[down];

    /* Left cell */
    int mn = (rr < d0) ? rr : d0;
    int mx = (rr > d0) ? rr : d0;
    if (!(l == mn || l == mx)) return false;

    /* Right cell */
    mn = (l < d1) ? l : d1;
    mx = (l > d1) ? l : d1;
    if (!(rr == mn || rr == mx)) return false;

    return true;
}

/* Check if row 'r' is valid as the last row with only an above neighbour.
 * 'up' is the row above 'r'. */
static inline bool last_row_ok(int r, int up)
{
    int l   = row_left[r];
    int rr  = row_right[r];
    int u0  = row_left[up];
    int u1  = row_right[up];

    /* Left cell */
    int mn = (rr < u0) ? rr : u0;
    int mx = (rr > u0) ? rr : u0;
    if (!(l == mn || l == mx)) return false;

    /* Right cell */
    mn = (l < u1) ? l : u1;
    mx = (l > u1) ? l : u1;
    if (!(rr == mn || rr == mx)) return false;

    return true;
}

/* Check if middle row 'mid' is valid given upper row 'up' and lower row 'down'. */
static inline bool middle_row_ok(int mid, int up, int down)
{
    int l   = row_left[mid];
    int rr  = row_right[mid];
    int u0  = row_left[up];
    int u1  = row_right[up];
    int d0  = row_left[down];
    int d1  = row_right[down];

    /* Left cell: neighbours right, up, down */
    int mn = rr;     int mx = rr;
    if (u0 < mn) mn = u0; if (u0 > mx) mx = u0;
    if (d0 < mn) mn = d0; if (d0 > mx) mx = d0;
    if (!(l == mn || l == mx)) return false;

    /* Right cell: neighbours left, up, down */
    mn = l;          mx = l;
    if (u1 < mn) mn = u1; if (u1 > mx) mx = u1;
    if (d1 < mn) mn = d1; if (d1 > mx) mx = d1;
    if (!(rr == mn || rr == mx)) return false;

    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* safety; the sequence is defined for n ≥ 1 */
    if (n == 1) return 3;        /* only uniform rows 00,11,22 satisfy the rule */

    /* DP arrays: dp[prev_prev * 9 + prev] holds count of sequences whose
     * last two rows are (prev_prev, prev), with all rows up to prev validated. */
    seq_t dp[81];
    seq_t ndp[81];

    /* initialise dp for rows 1 and 2 */
    for (int i = 0; i < 81; ++i) dp[i] = 0;
    for (int r1 = 0; r1 < 9; ++r1) {
        for (int r2 = 0; r2 < 9; ++r2) {
            if (first_row_ok(r1, r2))
                dp[r1 * 9 + r2] = 1;
        }
    }

    /* Add rows 3 .. n */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < 81; ++i) ndp[i] = 0;
        for (int prev_prev = 0; prev_prev < 9; ++prev_prev) {
            for (int prev = 0; prev < 9; ++prev) {
                seq_t ways = dp[prev_prev * 9 + prev];
                if (!ways) continue;
                for (int curr = 0; curr < 9; ++curr) {
                    if (middle_row_ok(prev, prev_prev, curr))
                        ndp[prev * 9 + curr] += ways;
                }
            }
        }
        /* move ndp -> dp */
        for (int i = 0; i < 81; ++i) dp[i] = ndp[i];
    }

    /* Validate last row with its above neighbour only */
    seq_t ans = 0;
    for (int prev = 0; prev < 9; ++prev) {
        for (int last = 0; last < 9; ++last) {
            if (dp[prev * 9 + last] && last_row_ok(last, prev))
                ans += dp[prev * 9 + last];
        }
    }
    return ans;
}
