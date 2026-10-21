/*
 * A206049  Number of (n+1)X4 0..2 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock equal to one.
 * https://oeis.org/A206049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A206049: Number of (n+1)×4 arrays of 0..2 where each 2×2 subblock has exactly one clockwise edge increase.
   Computed by dynamic programming over the 81 possible row patterns. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    enum { ROWS = 81 };
    static int rowVals[ROWS][4];
    static int neigh[ROWS][ROWS];
    static int neighCnt[ROWS];
    static bool init = false;

    if (!init) {
        /* Generate all 3⁴ row patterns in base‑3 representation */
        for (int r = 0; r < ROWS; ++r) {
            int v = r;
            for (int c = 3; c >= 0; --c) {
                rowVals[r][c] = v % 3;
                v /= 3;
            }
        }

        /* Compute compatibility between rows: every 2×2 subblock must have exactly one clockwise edge increase */
        for (int r = 0; r < ROWS; ++r) {
            int cnt = 0;
            for (int s = 0; s < ROWS; ++s) {
                bool ok = true;
                for (int col = 0; col < 3 && ok; ++col) {
                    int a = rowVals[r][col];
                    int b = rowVals[r][col + 1];
                    int c = rowVals[s][col];
                    int d = rowVals[s][col + 1];
                    int inc = (b > a) + (d > b) + (c > d) + (a > c);
                    if (inc != 1)
                        ok = false;
                }
                if (ok) {
                    neigh[r][cnt++] = s;
                }
            }
            neighCnt[r] = cnt;
        }
        init = true;
    }

    seq_t dp[ROWS];
    seq_t ndp[ROWS];
    for (int i = 0; i < ROWS; ++i)
        dp[i] = 1U;   /* any first row is allowed */

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < ROWS; ++i)
            ndp[i] = 0;
        for (int r = 0; r < ROWS; ++r) {
            seq_t val = dp[r];
            if (!val)
                continue;
            int cnt = neighCnt[r];
            for (int k = 0; k < cnt; ++k) {
                int s = neigh[r][k];
                ndp[s] += val;
            }
        }
        for (int i = 0; i < ROWS; ++i)
            dp[i] = ndp[i];
    }

    seq_t ans = 0;
    for (int i = 0; i < ROWS; ++i)
        ans += dp[i];
    return ans;
}
