/*
 * A198649  Number of n X 2 0..6 arrays with values 0..6 introduced in row
 *          major order and each element equal to at least one horizontal or
 *          vertical neighbor.
 * https://oeis.org/A198649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×2 0..6 arrays introduced in row major order
 *            with each element equal to at least one horizontal or vertical
 *            neighbor. Recurrence is implemented by dynamic programming over
 *            rows
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for idx_t and seq_t defined elsewhere */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, but harmless */

    /* DP arrays: dp[a][b][uL][uR][m] where
       a,b : cell values of the last processed row (0..6)
       uL,uR: 1 if the cell of last row is already satisfied,
               0 if it still needs a match from the next row
       m : maximum value used so far (0..6) */
    static seq_t dp_cur[7][7][2][2][7];
    static seq_t dp_next[7][7][2][2][7];
    int i, j, uL, uR, m, c, d;
    for (i = 0; i < 7; ++i)
        for (j = 0; j < 7; ++j)
            for (uL = 0; uL < 2; ++uL)
                for (uR = 0; uR < 2; ++uR)
                    for (m = 0; m < 7; ++m)
                        dp_cur[i][j][uL][uR][m] = 0;

    /* --- Initialize after processing the first row (row 1) --- */
    /* First cell is always 0 (introduces value 0) */
    /* Second cell can be 0 (same value) or 1 (new value, since m=0 before it) */
    /* Pattern (0,0) */
    dp_cur[0][0][1][1][0] = 1;  /* both cells satisfied horizontally, max = 0 */
    /* Pattern (0,1) */
    dp_cur[0][1][0][0][1] = 1;  /* both cells unsatisfied, max = 1 */

    /* --- Process rows 2 .. n --- */
    for (idx_t r = 2; r <= n; ++r) {
        /* zero dp_next */
        for (i = 0; i < 7; ++i)
            for (j = 0; j < 7; ++j)
                for (uL = 0; uL < 2; ++uL)
                    for (uR = 0; uR < 2; ++uR)
                        for (m = 0; m < 7; ++m)
                            dp_next[i][j][uL][uR][m] = 0;

        for (i = 0; i < 7; ++i)       /* previous left value a */
            for (j = 0; j < 7; ++j)   /* previous right value b */
                for (uL = 0; uL < 2; ++uL)
                    for (uR = 0; uR < 2; ++uR)
                        for (m = 0; m < 7; ++m) {
                            seq_t cnt = dp_cur[i][j][uL][uR][m];
                            if (!cnt) continue;

                            /* Possible values for current left cell c */
                            for (c = 0; c < 7; ++c) {
                                /* Must match above if previous cell was unsatisfied */
                                if (uL == 0 && c != i) continue;
                                /* Introduction order constraint for c */
                                if (!(c <= m || (c == m + 1 && c <= 6))) continue;
                                int m1 = m;
                                if (c > m1) m1 = c;

                                /* Possible values for current right cell d */
                                for (d = 0; d < 7; ++d) {
                                    if (uR == 0 && d != j) continue;
                                    if (!(d <= m1 || (d == m1 + 1 && d <= 6))) continue;
                                    int m2 = m1;
                                    if (d > m2) m2 = d;

                                    /* Satisfaction checks for current cells */
                                    int new_uL = (c == d || c == i) ? 1 : 0;
                                    int new_uR = (d == c || d == j) ? 1 : 0;

                                    dp_next[c][d][new_uL][new_uR][m2] += cnt;
                                }
                            }
                        }

        /* Swap dp_cur and dp_next by copying */
        for (i = 0; i < 7; ++i)
            for (j = 0; j < 7; ++j)
                for (uL = 0; uL < 2; ++uL)
                    for (uR = 0; uR < 2; ++uR)
                        for (m = 0; m < 7; ++m)
                            dp_cur[i][j][uL][uR][m] = dp_next[i][j][uL][uR][m];
    }

    /* --- Sum up states where the last row cells are satisfied --- */
    seq_t result = 0;
    for (i = 0; i < 7; ++i)
        for (j = 0; j < 7; ++j)
            for (m = 0; m < 7; ++m)
                result += dp_cur[i][j][1][1][m];

    return result;
}
