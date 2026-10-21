/*
 * A222890  Number of n X 1 0..7 arrays with no element equal to another at a
 *          city block distance of exactly two, and new values 0..7 introduced
 *          in row major order.
 * https://oeis.org/A222890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by dynamic programming. Let a(n) be the number of
 *            length‑n sequences over colors {0,..,7} such that 1. Colors are
 *            introduced in increasing order. At step t, the first new color
 *            that may appear is k, where k is the number of distinct colors
 *            used so far. 2. No two elements exactly two posi...
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 28, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* dp_cur[k][a][b] = number of sequences of current length
       that use k distinct colors (k>=1),
       the color at the penultimate position is a (0..7, or 8 for “none”),
       the color at the last position is b (0..7).               */
    static seq_t dp_cur[9][9][8];
    static seq_t dp_nxt[9][9][8];
    /* initialize for length 1: single element 0, one colour used,
       auxiliary ‘a’ is 8 meaning “no previous position”. */
    for (int k = 1; k <= 8; ++k)
        for (int a_col = 0; a_col <= 8; ++a_col)
            for (int b_col = 0; b_col < 8; ++b_col)
                dp_cur[k][a_col][b_col] = 0;
    dp_cur[1][8][0] = 1;

    if (n == 1) {
        /* only the initial state contributes */
        return 1;
    }

    /* iterate from length 1 up to n-1, adding one more element each time */
    for (idx_t len = 1; len < n; ++len) {
        /* clear next DP */
        for (int k = 1; k <= 8; ++k)
            for (int a_col = 0; a_col <= 8; ++a_col)
                for (int b_col = 0; b_col < 8; ++b_col)
                    dp_nxt[k][a_col][b_col] = 0;

        for (int k = 1; k <= 8; ++k) {
            for (int a_col = 0; a_col <= 8; ++a_col) {
                for (int b_col = 0; b_col < 8; ++b_col) {
                    seq_t cur_val = dp_cur[k][a_col][b_col];
                    if (cur_val == 0) continue;

                    /* option 1: use an existing colour c < k */
                    for (int c = 0; c < k; ++c) {
                        if (c == a_col) continue;          /* avoid distance‑2 equality */
                        dp_nxt[k][b_col][c] += cur_val;
                    }

                    /* option 2: introduce a new colour k (if we have room) */
                    if (k < 8) {
                        int newc = k;                      /* new colour id */
                        if (newc != a_col) {               /* distance‑2 check */
                            dp_nxt[k + 1][b_col][newc] += cur_val;
                        }
                    }
                }
            }
        }

        /* swap dp_cur and dp_nxt for next iteration */
        for (int k = 1; k <= 8; ++k)
            for (int a_col = 0; a_col <= 8; ++a_col)
                for (int b_col = 0; b_col < 8; ++b_col)
                    dp_cur[k][a_col][b_col] = dp_nxt[k][a_col][b_col];
    }

    /* sum all states for the final length */
    seq_t result = 0;
    for (int k = 1; k <= 8; ++k)
        for (int a_col = 0; a_col <= 8; ++a_col)
            for (int b_col = 0; b_col < 8; ++b_col)
                result += dp_cur[k][a_col][b_col];

    return result;
}
