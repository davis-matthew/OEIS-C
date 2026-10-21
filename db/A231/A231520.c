/*
 * A231520  Number of nX5 0..1 arrays with no element less than a strict
 *          majority of its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A231520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A231520 – number of n×5 binary arrays with
   no element less than a strict majority of its
   horizontal, diagonal and antidiagonal neighbours.
   Computed by a transfer–matrix DP. */

#include <stdbool.h>
#include <stdint.h>

#ifndef NONE_ROW
#define NONE_ROW 32                        /* special id for “missing” row   */
#endif

/* helper: test whether the middle row (mid) is valid given
   the row above (up) and the row below (down).              */
static bool row_valid(int up, int mid, int down)
{
    /* mid is a 5‑bit pattern, bit c (0..4) is the cell value at column c   */
    for (int c = 0; c < 5; ++c) {
        int sum = 0;     /* sum of ones among neighbours                */
        int cnt = 0;     /* number of existing neighbours               */

        /* horizontal neighbours */
        if (c > 0) {
            sum += (mid >> (c-1)) & 1;
            ++cnt;
        }
        if (c < 4) {
            sum += (mid >> (c+1)) & 1;
            ++cnt;
        }

        /* diagonal neighbours from the row above */
        if (up != NONE_ROW) {
            if (c > 0) {
                sum += (up >> (c-1)) & 1;
                ++cnt;
            }
            if (c < 4) {
                sum += (up >> (c+1)) & 1;
                ++cnt;
            }
        }

        /* diagonal neighbours from the row below */
        if (down != NONE_ROW) {
            if (c > 0) {
                sum += (down >> (c-1)) & 1;
                ++cnt;
            }
            if (c < 4) {
                sum += (down >> (c+1)) & 1;
                ++cnt;
            }
        }

        /* strict majority of neighbours is 1 ?                */
        if ((sum * 2) > cnt) {                      /* majority = 1 */
            if (((mid >> c) & 1) == 0)             /* but cell is 0 */
                return false;                     /* violates rule */
        }
    }
    return true;
}

/* sequence entry function */
seq_t a(idx_t n)
{
    /* trivial case: offset starts at 1, nothing defined for n < 1 */
    if (n < 1) return (seq_t)0;

    /* pre‑compute validity table once */
    static bool valid[33][33][33];
    static bool computed = false;
    if (!computed) {
        for (int up = 0; up <= 32; ++up)
            for (int mid = 0; mid <= 31; ++mid)
                for (int down = 0; down <= 32; ++down)
                    valid[up][mid][down] = row_valid(up, mid, down);
        computed = true;
    }

    /* DP tables: dp[prevprev][prev]  stores counts of arrays
       where rows 1..current are built and rows up to current-1
       are already valid.                                       */
    static seq_t dp[33][33];
    static seq_t nextdp[33][33];
    for (int i = 0; i < 33; ++i)
        for (int j = 0; j < 33; ++j)
            dp[i][j] = 0;

    /* initialise with first row (n = 1) */
    for (int p = 0; p < 32; ++p)
        dp[NONE_ROW][p] = 1;

    if (n == 1) {
        seq_t result = 0;
        for (int prev = 0; prev < 32; ++prev)
            if (valid[NONE_ROW][prev][NONE_ROW])
                result += dp[NONE_ROW][prev];
        return result;
    }

    /* iterate over remaining rows (2 .. n) */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero nextdp */
        for (int i = 0; i < 33; ++i)
            for (int j = 0; j < 33; ++j)
                nextdp[i][j] = 0;

        /* transition */
        for (int up = 0; up <= 32; ++up) {
            for (int mid = 0; mid <= 31; ++mid) {
                seq_t curcnt = dp[up][mid];
                if (curcnt == 0) continue;
                for (int down = 0; down < 32; ++down) {
                    if (!valid[up][mid][down]) continue;
                    nextdp[mid][down] += curcnt;
                }
            }
        }
        /* swap dp tables */
        for (int i = 0; i < 33; ++i)
            for (int j = 0; j < 33; ++j)
                dp[i][j] = nextdp[i][j];
    }

    /* final validation for the last row */
    seq_t result = 0;
    for (int up = 0; up <= 32; ++up) {
        for (int mid = 0; mid <= 31; ++mid) {
            if (!valid[up][mid][NONE_ROW]) continue;
            result += dp[up][mid];
        }
    }
    return result;
}
