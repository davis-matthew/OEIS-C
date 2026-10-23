/*
 * A188691  Number of nX5 binary arrays without the pattern 0 1 0 diagonally,
 *          vertically, antidiagonally or horizontally.
 * https://oeis.org/A188691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by dynamic programming over the rows of a 5‑column
 *            binary array; the function counts n×5 matrices that do not
 *            contain a 0‑1‑0 pattern in any horizontal, vertical, diagonal
 *            (↘) or anti‑diagonal (↙) direction
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses constant‑size static tables whose bounds are
   known at compile time via enum constants.  All counting arithmetic is
   performed in seq_t; no external libraries are required. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Dimension constants */
    enum { MASKS  = 32,            /* 5‑bit masks 0 … 31 */
           SENTINEL = MASKS,       /* special value for “no previous row” */
           STATES = MASKS + 1 };   /* 0 … 31 plus sentinel */

    /* Static data built once */
    static bool built = false;

    /* Allowed rows (no horizontal 010) */
    static int allow_rows[MASKS];
    static int allow_cnt = 0;

    /* Transition tables */
    static int trans_cnt[STATES][STATES];
    static int trans_list[STATES][STATES][MASKS];

    if (!built) {
        /* Compute all allowed rows (no horizontal 010) */
        allow_cnt = 0;
        for (int mask = 0; mask < MASKS; ++mask) {
            bool ok = true;
            for (int j = 0; j <= 2; ++j) {
                if (((mask >> j) & 7) == 0b010) {
                    ok = false;
                    break;
                }
            }
            if (ok)
                allow_rows[allow_cnt++] = mask;
        }

        /* Precompute transition possibilities for all states */
        for (int a = 0; a < STATES; ++a) {
            for (int b = 0; b < STATES; ++b) {
                int cnt = 0;
                if (a == SENTINEL || b == SENTINEL) {
                    /* If we are at the top of the grid, any allowed row works */
                    for (int k = 0; k < allow_cnt; ++k)
                        trans_list[a][b][cnt++] = allow_rows[k];
                } else {
                    /* All three rows are real: enforce vertical and diagonal
                       0‑1‑0 restrictions. */
                    for (int k = 0; k < allow_cnt; ++k) {
                        int c = allow_rows[k];
                        bool ok = true;

                        /* Vertical 010 */
                        for (int j = 0; j < 5 && ok; ++j) {
                            int bit_a = (a >> j) & 1;
                            int bit_b = (b >> j) & 1;
                            int bit_c = (c >> j) & 1;
                            if (bit_a == 0 && bit_b == 1 && bit_c == 0)
                                ok = false;
                        }

                        /* Diagonal ↘ (top‑left → bottom‑right) */
                        for (int j = 0; j <= 2 && ok; ++j) {
                            int bit_a = (a >> j) & 1;
                            int bit_b = (b >> (j + 1)) & 1;
                            int bit_c = (c >> (j + 2)) & 1;
                            if (bit_a == 0 && bit_b == 1 && bit_c == 0)
                                ok = false;
                        }

                        /* Anti‑diagonal ↙ (top‑right → bottom‑left) */
                        for (int j = 2; j < 5 && ok; ++j) {
                            int bit_a = (a >> j) & 1;
                            int bit_b = (b >> (j - 1)) & 1;
                            int bit_c = (c >> (j - 2)) & 1;
                            if (bit_a == 0 && bit_b == 1 && bit_c == 0)
                                ok = false;
                        }

                        if (ok)
                            trans_list[a][b][cnt++] = c;
                    }
                }
                trans_cnt[a][b] = cnt;
            }
        }

        built = true;
    }

    /* DP tables: dp[a][b] counts ways to reach a state where the
       previous row is a and the current last row is b. */
    static seq_t dp[STATES][STATES];
    static seq_t ndp[STATES][STATES];

    /* Initialize DP with empty start state */
    for (int a = 0; a < STATES; ++a)
        for (int b = 0; b < STATES; ++b)
            dp[a][b] = 0;
    dp[SENTINEL][SENTINEL] = 1;

    /* Process each of the n rows */
    for (idx_t i = 1; i <= n; ++i) {
        /* Clear next DP state */
        for (int a = 0; a < STATES; ++a)
            for (int b = 0; b < STATES; ++b)
                ndp[a][b] = 0;

        /* Transition to next row */
        for (int a = 0; a < STATES; ++a) {
            for (int b = 0; b < STATES; ++b) {
                seq_t val = dp[a][b];
                if (val == 0) continue;
                int cnt = trans_cnt[a][b];
                for (int t = 0; t < cnt; ++t) {
                    int c = trans_list[a][b][t];
                    ndp[b][c] = ndp[b][c] + val;
                }
            }
        }

        /* Copy next state back to dp for the following iteration */
        for (int a = 0; a < STATES; ++a)
            for (int b = 0; b < STATES; ++b)
                dp[a][b] = ndp[a][b];
    }

    /* Sum all final states where the last row is real (0 … 31) */
    seq_t total = 0;
    for (int a = 0; a < STATES; ++a)
        for (int b = 0; b < MASKS; ++b)
            total = total + dp[a][b];

    return total;
}
