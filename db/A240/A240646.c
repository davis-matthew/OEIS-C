/*
 * A240646  Number of nX5 0..1 arrays with no element equal to the same number
 *          of vertical neighbors as horizontal neighbors, with new values
 *          0..1 introduced in row major order.
 * https://oeis.org/A240646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   count_valid_rows_with_first_zero, row_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computing OEIS A240646: number of n×5 binary arrays with no element having
   equal numbers of vertical and horizontal neighbors that contain the same
   value, and with the first cell (top-left) forced to be 0 (new values
   introduced in row-major order).  The program implements a DP over pairs of
   consecutive rows. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/count_valid_rows_with_first_zero.h"
#include "../utils/row_ok.h"

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* safety for index 0 */

    /* Build transition masks once */
    static bool init_masks = false;
    static uint32_t mask_next[32][32];
    if (!init_masks) {
        init_masks = true;
        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                uint32_t m = 0;
                for (int c = 0; c < 32; ++c) {
                    if (row_ok(b, a, c, true, true))
                        m |= (1u << c);
                }
                mask_next[a][b] = m;
            }
        }
    }

    /* Special case n==1: only the first row matters */
    if (n == 1)
        return count_valid_rows_with_first_zero();

    /* DP arrays for pairs of consecutive rows */
    static seq_t dp[32][32];
    static seq_t dp_next[32][32];

    /* clear dp */
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            dp[a][b] = 0;

    /* initialization: first two rows, enforce first cell 0*/
    for (int r0 = 0; r0 < 32; ++r0) {
        if ((r0 & 1) == 0) { /* top-left must be 0 */
            for (int r1 = 0; r1 < 32; ++r1) {
                if (row_ok(r0, 0, r1, false, true))
                    dp[r0][r1] = (seq_t)1;
            }
        }
    }

    /* Add rows 3 .. n (1-based).  We already have rows 1 and 2. */
    for (idx_t step = 2; step < n; ++step) {
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                dp_next[a][b] = 0;

        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                seq_t cur = dp[a][b];
                if (!cur) continue;
                uint32_t m = mask_next[a][b];
                for (int c = 0; c < 32; ++c) {
                    if (m & (1u << c))
                        dp_next[b][c] += cur;
                }
            }
        }

        /* swap dp and dp_next */
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                dp[a][b] = dp_next[a][b];
    }

    /* Sum over final states: last row must be valid against non-existent
       bottom neighbor. */
    seq_t result = 0;
    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            seq_t cur = dp[a][b];
            if (!cur) continue;
            if (row_ok(b, a, 0, true, false))
                result += cur;
        }
    }

    return result;
}
