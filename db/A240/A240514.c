/*
 * A240514  Number of nX3 0..1 arrays with no element equal to exactly two
 *          horizontal and vertical neighbors, with new values 0..1 introduced
 *          in row major order.
 * https://oeis.org/A240514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 binary arrays with no element equal to
 *            exactly two of its horizontal and vertical neighbours, subject
 *            to the first element (row 0, column 0) being 0. The DP below
 *            counts all arrays satisfying the rule and automatically enforces
 *            the first‑cell constraint
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool valid_row(int up, int mid, int down)
{
    for (int col = 0; col < 3; ++col) {
        int val_mid = (mid >> col) & 1;
        int same = 0;

        if (col > 0) {
            int left = (mid >> (col - 1)) & 1;
            if (left == val_mid) ++same;
        }
        if (col < 2) {
            int right = (mid >> (col + 1)) & 1;
            if (right == val_mid) ++same;
        }
        if (up != 8) {
            int up_bit = (up >> col) & 1;
            if (up_bit == val_mid) ++same;
        }
        if (down != 8) {
            int down_bit = (down >> col) & 1;
            if (down_bit == val_mid) ++same;
        }
        if (same == 2) return false;   /* forbidden */
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Special case: a(0) is undefined; treat n >= 1 only. */
    if (n == 0) return 0;

    /* Count arrays where the first cell (row 0, col 0) is 0. */
    /* n == 1 : only the first row matters. */
    if (n == 1) {
        seq_t res = 0;
        for (int pat = 0; pat < 8; ++pat) {
            if ((pat & 1) == 0 && /* leftmost bit (col 0) is 0 */
                valid_row(8, pat, 8))
                ++res;
        }
        return res;
    }

    /* DP state: dp_cur[p_prev][p_curr] = number of ways to
       realise the last two rows (p_prev, p_curr) with p_prev already
       validated. */
    seq_t dp_cur[8][8] = {0};
    seq_t dp_next[8][8];

    /* Initialize with all first‑row patterns that start with 0,
       and with every possible second row that validates the first. */
    for (int p1 = 0; p1 < 8; ++p1) {
        if ((p1 & 1) != 0) continue;          /* first cell must be 0 */
        for (int p2 = 0; p2 < 8; ++p2) {
            if (valid_row(8, p1, p2))
                dp_cur[p1][p2] = 1;
        }
    }

    /* Add rows 3 … n, validating the middle row each time. */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp_next[i][j] = 0;

        for (int prev = 0; prev < 8; ++prev) {
            for (int curr = 0; curr < 8; ++curr) {
                seq_t w = dp_cur[prev][curr];
                if (!w) continue;
                for (int next = 0; next < 8; ++next) {
                    if (!valid_row(prev, curr, next)) continue;
                    dp_next[curr][next] += w;
                }
            }
        }
        /* copy dp_next back to dp_cur for next iteration */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp_cur[i][j] = dp_next[i][j];
    }

    /* Final validation of the last row (no down neighbour). */
    seq_t result = 0;
    for (int prev = 0; prev < 8; ++prev) {
        for (int curr = 0; curr < 8; ++curr) {
            if (valid_row(prev, curr, 8))
                result += dp_cur[prev][curr];
        }
    }

    return result;
}
