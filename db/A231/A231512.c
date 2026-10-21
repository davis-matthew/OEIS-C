/*
 * A231512  Number of nX5 0..1 arrays with no element less than a strict
 *          majority of its horizontal, vertical, diagonal and antidiagonal
 *          neighbors.
 * https://oeis.org/A231512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A231512: number of n×5 0/1 arrays where no 0 has a strict majority
   of its eight neighbors (horizontal, vertical, diagonal, antidiagonal).
   Algorithm: Dynamic programming over rows, keeping the last two rows as a state.
   For each intermediate row we verify the conditions for the middle row of the
   3‑row window. For the final answer we additionally check the last row. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define STATE_COUNT 32  /* 2^5 possible rows */

/* Return true if the middle row satisfies the condition given the neighboring
   rows.  has_above/below indicate whether the respective neighboring row
   exists.  All rows are represented as 5‑bit integers. */
static inline bool valid_row(unsigned int above, unsigned int mid, unsigned int below,
                             bool has_above, bool has_below)
{
    for (int col = 0; col < 5; ++col) {
        int mid_bit = (mid >> col) & 1;
        if (mid_bit == 0) {
            int degree = 0, ones = 0;

            if (has_above) {
                if (col > 0) { degree++; ones += (above >> (col - 1)) & 1; }
                degree++;     ones += (above >> col) & 1;
                if (col < 4) { degree++; ones += (above >> (col + 1)) & 1; }
            }
            /* neighbors in the same row */
            if (col > 0) { degree++; ones += (mid >> (col - 1)) & 1; }
            if (col < 4) { degree++; ones += (mid >> (col + 1)) & 1; }

            if (has_below) {
                if (col > 0) { degree++; ones += (below >> (col - 1)) & 1; }
                degree++;     ones += (below >> col) & 1;
                if (col < 4) { degree++; ones += (below >> (col + 1)) & 1; }
            }

            /* If 0 has a strict majority of 1s among its neighbors -> invalid */
            if (2 * ones > degree) return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for zero, return 0 for safety */

    /* Special case n == 1: only one row, no below or above rows. */
    if (n == 1) {
        seq_t total = 0;
        for (unsigned int r = 0; r < STATE_COUNT; ++r) {
            if (valid_row(0, r, 0, false, false))
                total += 1;
        }
        return total;
    }

    /* DP arrays: dp[prev2][prev1] counts for the last two rows processed. */
    static seq_t dp[STATE_COUNT][STATE_COUNT];
    static seq_t newdp[STATE_COUNT][STATE_COUNT];

    /* initialise dp to 0 */
    for (idx_t i = 0; i < STATE_COUNT; ++i)
        for (idx_t j = 0; j < STATE_COUNT; ++j)
            dp[i][j] = 0;

    /* initial state: first two rows (r0, r1).  Check row 0 with only below row. */
    for (idx_t r0 = 0; r0 < STATE_COUNT; ++r0)
        for (idx_t r1 = 0; r1 < STATE_COUNT; ++r1)
            if (valid_row(0, (unsigned int)r0, (unsigned int)r1, false, true))
                dp[r0][r1] = 1;

    /* Process rows 2 .. n-1 */
    for (idx_t row = 2; row < n; ++row) {
        /* zero newdp */
        for (idx_t i = 0; i < STATE_COUNT; ++i)
            for (idx_t j = 0; j < STATE_COUNT; ++j)
                newdp[i][j] = 0;

        for (idx_t prev2 = 0; prev2 < STATE_COUNT; ++prev2)
            for (idx_t prev1 = 0; prev1 < STATE_COUNT; ++prev1) {
                seq_t ways = dp[prev2][prev1];
                if (ways == 0) continue;
                for (idx_t cur = 0; cur < STATE_COUNT; ++cur) {
                    if (valid_row((unsigned int)prev2, (unsigned int)prev1,
                                  (unsigned int)cur, true, true)) {
                        newdp[prev1][cur] += ways;
                    }
                }
            }

        /* copy newdp into dp for next iteration */
        for (idx_t i = 0; i < STATE_COUNT; ++i)
            for (idx_t j = 0; j < STATE_COUNT; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* After all rows processed, check the final (last) row for validity. */
    seq_t result = 0;
    for (idx_t prev2 = 0; prev2 < STATE_COUNT; ++prev2)
        for (idx_t prev1 = 0; prev1 < STATE_COUNT; ++prev1) {
            seq_t ways = dp[prev2][prev1];
            if (ways == 0) continue;
            if (valid_row((unsigned int)prev2, (unsigned int)prev1,
                          0, true, false)) {
                result += ways;
            }
        }
    return result;
}
