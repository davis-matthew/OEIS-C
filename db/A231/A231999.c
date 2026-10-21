/*
 * A231999  Number of (2+1)X(n+1) 0..1 arrays with no element having a strict
 *          majority of its horizontal and antidiagonal neighbors equal to
 *          one.
 * https://oeis.org/A231999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   column_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursively computed via dynamic‑programming over columns.
   For a 3×(n+1) binary array the constraint is:
   for each cell, among its horizontal and antidiagonal neighbours
   (only those that exist) a strict majority of ones is forbidden.
   The function a(n) returns the number of such arrays for index n
   (offset 1). */
#include <stdbool.h>
#include "../utils/column_valid.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t m = n + 1;                 /* number of columns */

    seq_t dp[8][8] = {0};
    seq_t newdp[8][8];

    /* initial step: column 0 (prev absent), column 1 */
    for (int col0 = 0; col0 < 8; ++col0)
        for (int col1 = 0; col1 < 8; ++col1)
            if (column_valid(0, false, col0, col1, true))
                dp[col0][col1] = 1;

    /* transition through interior columns (2 … m‑1) */
    for (idx_t col = 2; col <= m - 1; ++col) {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                newdp[i][j] = 0;

        for (int prev = 0; prev < 8; ++prev) {
            for (int curr = 0; curr < 8; ++curr) {
                seq_t ways = dp[prev][curr];
                if (!ways) continue;
                for (int nxt = 0; nxt < 8; ++nxt) {
                    if (column_valid(prev, true, curr, nxt, true))
                        newdp[curr][nxt] += ways;
                }
            }
        }
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* final column: no right neighbour */
    seq_t total = 0;
    for (int prev = 0; prev < 8; ++prev) {
        for (int curr = 0; curr < 8; ++curr) {
            seq_t ways = dp[prev][curr];
            if (!ways) continue;
            if (column_valid(prev, true, curr, 0, false))
                total += ways;
        }
    }
    return total;
}
