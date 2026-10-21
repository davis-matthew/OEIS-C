/*
 * A208195  Number of n X 2 0..3 arrays with new values 0..3 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or left-upward diagonal neighbors.
 * https://oeis.org/A208195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A208195
 * 
 * The array is built row by row (row-major order). For each cell we must
 * obey two rules:
 * 1. **Introduction order** – the first time we use a new value it must be
 *    the smallest value that has not yet appeared.  Consequently, at any
 *    point the set of unused values is a contiguous prefix of {0,1,2,3}.
 * 2. **Neighbour constraint** – an element may not be equal to all three of
 *    its existing neighbours (left, up, up‑left).  Thus for a cell with all
 *    three neighbours defined, the value can be equal to at most two of
 *    them; if the three neighbours are all equal to a value v, the current
 *    cell may not be v.
 *
 * We perform a dynamic‑programming over the state
 *     (previous_row_left, previous_row_right, mask_of_used_values)
 * where `mask_of_used_values` is a 4‑bit bitmask of values that have
 * already appeared.  The transition considers all possible values for the
 * left and right cells of the new row, respecting the introduction order
 * and the neighbour constraint, and updates the mask accordingly.
 *
 * The algorithm runs in O(n * 4^3 * 2^4) ≈ 40 000 operations per row,
 * easily within the required time for n up to 10 000. */

#include <stdint.h>
#include <string.h>

/* Assume seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1 */
    /* dp[cur][left][right][mask] */
    static seq_t dp_cur[4][4][16];
    static seq_t dp_next[4][4][16];
    /* clear dp_cur for the first row */
    memset(dp_cur, 0, sizeof(dp_cur));

    /* ----- Row 1 --------------------------------------------------- */
    /* In row 1 the neighbours are non‑existent, so the only restriction
     * is the introduction order: the first value must be 0.  After
     * placing 0 the mask becomes 1.  The second cell can be 0 or 1. */
    dp_cur[0][0][1] = 1;               /* row values (0,0)  mask 0b0001 */
    dp_cur[0][1][3] = 1;               /* row values (0,1)  mask 0b0011 */

    /* ----- Rows 2 .. n ------------------------------------------------ */
    for (idx_t row = 2; row <= n; ++row) {
        /* clear dp_next */
        memset(dp_next, 0, sizeof(dp_next));

        for (int prev_l = 0; prev_l < 4; ++prev_l) {
            for (int prev_r = 0; prev_r < 4; ++prev_r) {
                for (int mask = 0; mask < 16; ++mask) {
                    seq_t cur_val = dp_cur[prev_l][prev_r][mask];
                    if (cur_val == 0) continue;

                    /* Find next unused value for current mask */
                    int next_unused = 4;
                    for (int v = 0; v < 4; ++v)
                        if (!(mask & (1 << v))) { next_unused = v; break; }

                    /* Choose value for left cell (current row) */
                    for (int c = 0; c < 4; ++c) {
                        if (!(mask & (1 << c)) && c != next_unused) continue;
                        int mask1 = mask | (1 << c);

                        /* Compute next unused after placing c */
                        int next_unused1 = 4;
                        for (int v = 0; v < 4; ++v)
                            if (!(mask1 & (1 << v))) { next_unused1 = v; break; }

                        /* Choose value for right cell */
                        for (int d = 0; d < 4; ++d) {
                            if (!(mask1 & (1 << d)) && d != next_unused1) continue;

                            /* Neighbour constraint: if all three neighbours
                               (prev_l, prev_r, c) are equal to v, d cannot be v. */
                            if (prev_l == prev_r && prev_r == c && d == c)
                                continue;

                            int mask2 = mask1 | (1 << d);
                            dp_next[c][d][mask2] += cur_val;
                        }
                    }
                }
            }
        }
        /* swap dp_cur and dp_next for next iteration */
        memcpy(dp_cur, dp_next, sizeof(dp_cur));
    }

    /* ----- Sum over all states for the last row ---------------------- */
    seq_t result = 0;
    for (int l = 0; l < 4; ++l)
        for (int r = 0; r < 4; ++r)
            for (int m = 0; m < 16; ++m)
                result += dp_cur[l][r][m];

    return result;
}
