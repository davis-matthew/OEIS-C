/*
 * A240632  Number of nX4 0..1 arrays with no element equal to exactly three
 *          horizontal or vertical neighbors, with new values 0..1 introduced
 *          in row major order.
 * https://oeis.org/A240632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A240632:
   a(n) = number of n×4 binary arrays with no element equal to exactly
   three of its orthogonal neighbors, with the first cell fixed to 0
   (row‑major order).  Implemented by dynamic programming over rows. */

#include <stdbool.h>

static const idx_t NONE = 16;          /* sentinel for “no neighbour” */

/* Return true iff each cell of `row` does **not** have exactly three
   equal orthogonal neighbours, given the up and down neighbour rows.
   `up` or `down` equal to NONE indicates absence of that neighbour. */
static bool row_ok(idx_t row, idx_t up, idx_t down)
{
    for (idx_t j = 0; j < 4; ++j) {
        int same = 0;
        int bit = (row >> j) & 1;

        /* horizontal neighbours */
        if (j > 0 && ((row >> (j - 1)) & 1) == bit) ++same;
        if (j < 3 && ((row >> (j + 1)) & 1) == bit) ++same;

        /* vertical neighbours, if present */
        if (up   != NONE && ((up   >> j) & 1) == bit) ++same;
        if (down != NONE && ((down >> j) & 1) == bit) ++same;

        if (same == 3) return false;   /* forbidden */
    }
    return true;
}

seq_t a(idx_t n)
{
    /* n == 0 is undefined by OEIS, return 0 for safety */
    if (n == 0) return 0;

    /* n == 1: the only restriction is the fixed first cell (0,0)=0 */
    if (n == 1) return (seq_t)8;   /* 2^3 = 8 valid rows */

    /* DP arrays: dp[prev_row][curr_row] counts ways after the current row
       is fixed.  Rows are 4‑bit patterns 0..15. */
    seq_t dp[16][16] = {{0}};
    seq_t ndp[16][16] = {{0}};

    /* --- initialise with the first two rows --- */
    for (idx_t r0 = 0; r0 < 16; ++r0) {
        if ((r0 & 1) != 0) continue;          /* first cell fixed to 0 */
        for (idx_t r1 = 0; r1 < 16; ++r1) {
            if (row_ok(r0, NONE, r1))
                dp[r0][r1] = 1;               /* only one way to choose */
        }
    }

    /* --- add rows 2 .. n-1 --- */
    for (idx_t i = 2; i < n; ++i) {
        for (idx_t p = 0; p < 16; ++p)
            for (idx_t c = 0; c < 16; ++c)
                ndp[p][c] = 0;

        for (idx_t prev = 0; prev < 16; ++prev) {
            for (idx_t curr = 0; curr < 16; ++curr) {
                seq_t val = dp[prev][curr];
                if (!val) continue;
                for (idx_t next = 0; next < 16; ++next) {
                    if (row_ok(curr, prev, next))
                        ndp[curr][next] += val;
                }
            }
        }

        /* move ndp to dp for the next iteration */
        for (idx_t p = 0; p < 16; ++p)
            for (idx_t c = 0; c < 16; ++c)
                dp[p][c] = ndp[p][c];
    }

    /* --- final row has no down neighbour --- */
    seq_t result = 0;
    for (idx_t prev = 0; prev < 16; ++prev) {
        for (idx_t curr = 0; curr < 16; ++curr) {
            seq_t val = dp[prev][curr];
            if (!val) continue;
            if (row_ok(curr, prev, NONE))
                result += val;
        }
    }
    return result;
}
