/*
 * A202485  Number of (n+2) X 3 binary arrays avoiding patterns 000 and 010 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 68,
 *            big any n
 * Helpers:   forbidden_pattern
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A202485: a(n) = number of (n+2)×3 binary arrays avoiding
   the patterns 000 and 010 in rows, columns and NW‑to‑SE diagonals. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/forbidden_pattern.h"

/* checks a triple of rows for vertical columns and the main NW‑SE
   diagonal of length 3 */
static inline bool triple_valid(int r0, int r1, int r2)
{
    /* vertical columns */
    for (int j = 0; j < 3; ++j) {
        int a = (r0 >> j) & 1;
        int b = (r1 >> j) & 1;
        int c = (r2 >> j) & 1;
        if (forbidden_pattern(a, b, c))
            return false;
    }
    /* NW–SE main diagonal: (row0,col0),(row1,col1),(row2,col2) */
    int a0 = r0 & 1;
    int b1 = (r1 >> 1) & 1;
    int c2 = (r2 >> 2) & 1;
    return !forbidden_pattern(a0, b1, c2);
}

seq_t a(idx_t n)
{
    /* height of the array */
    idx_t N = n + 2;

    /* allowed 3‑bit row patterns (exclude 000 and 010) */
    bool allowed[8];
    for (int i = 0; i < 8; ++i)
        allowed[i] = (i != 0 && i != 2);

    /* dp[prev][cur] counts ways for the last two rows = (prev,cur) */
    seq_t dp[8][8] = {0};

    /* initialise for the first two rows */
    for (int r0 = 0; r0 < 8; ++r0) {
        if (!allowed[r0]) continue;
        for (int r1 = 0; r1 < 8; ++r1) {
            if (!allowed[r1]) continue;
            dp[r0][r1] = 1;
        }
    }

    /* iterate over remaining rows (from the 3rd row to the N‑th row) */
    for (idx_t step = 2; step < N; ++step) {
        seq_t next[8][8];
        /* zero initialise the next table */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                next[i][j] = 0;

        for (int r0 = 0; r0 < 8; ++r0) {
            if (!allowed[r0]) continue;
            for (int r1 = 0; r1 < 8; ++r1) {
                if (!allowed[r1]) continue;
                seq_t val = dp[r0][r1];
                if (!val) continue;
                for (int r2 = 0; r2 < 8; ++r2) {
                    if (!allowed[r2]) continue;
                    if (!triple_valid(r0, r1, r2))
                        continue;
                    next[r1][r2] += val;
                }
            }
        }

        /* copy next to dp for the next iteration */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = next[i][j];
    }

    /* sum all possibilities for the last two rows */
    seq_t total = 0;
    for (int r0 = 0; r0 < 8; ++r0) {
        if (!allowed[r0]) continue;
        for (int r1 = 0; r1 < 8; ++r1) {
            if (!allowed[r1]) continue;
            total += dp[r0][r1];
        }
    }

    return total;
}
