/*
 * A202310  Number of (n+2) X 3 binary arrays avoiding patterns 001 and 100 in
 *          rows and columns.
 * https://oeis.org/A202310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A202310: Number of (n+2) × 3 binary arrays avoiding the
 * binary patterns 001 and 100 in every row and every column.
 * Computed by dynamic programming over rows.
 *
 * Algorithm:
 *   * A row may be any 3‑bit string except 001 (1) and 100 (4);
 *   * while adding a new row, the last two real rows together with
 *     the new row must not contain a vertical 001 or 100 in any
 *     column.
 *   * We keep a DP table dp[prev2][prev1] of counts of ways to keep
 *     the last two rows as “prev2” and “prev1”.
 *     For the very first row we keep a one‑dimensional array dp1[prev1].
 *   * The transition from row i to i+1 uses the vertical check only
 *     from i≥3 onward.  This avoids mixing the dummy “start” state
 *     with a real row value 0.
 */

#include <stdbool.h>

/* allowed rows in a 3×1 column (binary strings excluding 001 and 100) */
static const int allowed_rows[6] = {0, 2, 3, 5, 6, 7};   /* 000,010,011,101,110,111 */

/* Return true iff the column triple (a,b,c) does NOT contain 001 or 100. */
static inline bool vertical_ok(int a, int b, int c)
{
    for (int j = 0; j < 3; ++j) {
        int aj = (a >> j) & 1;
        int bj = (b >> j) & 1;
        int cj = (c >> j) & 1;
        if ((aj == 0 && bj == 0 && cj == 1) ||
            (aj == 1 && bj == 0 && cj == 0)) {
            return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    /* number of rows in the array */
    idx_t rows = n + 2;

    /* one‑row DP (after the first real row) */
    seq_t dp1[8] = {0};

    /* initialise dp1 with all allowed rows */
    for (int r = 0; r < 6; ++r) {
        dp1[allowed_rows[r]] = 1;
    }

    /* two‑row DP, first iteration handles row 2, subsequent iterations
     * handle rows 3 … rows.  dp2 holds states (prev2, prev1). */
    seq_t dp2[8][8] = {{0}};
    seq_t nextdp[8][8] = {{0}};

    for (idx_t row = 2; row <= rows; ++row) {
        /* clear nextdp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                nextdp[i][j] = 0;

        if (row == 2) {
            /* transition from dp1 (first row) to dp2 (rows 1‑2) */
            for (int prev = 0; prev < 8; ++prev) {
                seq_t ways = dp1[prev];
                if (ways == 0) continue;
                for (int k = 0; k < 6; ++k) {
                    int newr = allowed_rows[k];
                    nextdp[prev][newr] += ways;
                }
            }
        } else {
            /* transition from dp2 (rows 1‑(row-1)) to dp2 (rows 2‑row) */
            for (int p1 = 0; p1 < 8; ++p1) {
                for (int p2 = 0; p2 < 8; ++p2) {
                    seq_t ways = dp2[p1][p2];
                    if (ways == 0) continue;
                    for (int k = 0; k < 6; ++k) {
                        int newr = allowed_rows[k];
                        if (vertical_ok(p1, p2, newr)) {
                            nextdp[p2][newr] += ways;
                        }
                    }
                }
            }
        }

        /* copy nextdp into dp2 for the next iteration */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp2[i][j] = nextdp[i][j];
    }

    /* sum all final states */
    seq_t result = 0;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            result += dp2[i][j];

    return result;
}
