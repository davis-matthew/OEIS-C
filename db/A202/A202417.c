/*
 * A202417  Number of (n+2)X6 binary arrays with no more than one of any
 *          consecutive three bits set in any row or column.
 * https://oeis.org/A202417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×6 binary arrays with no more than one of
 *            any consecutive three bits set in any row or column. Computed by
 *            a simple dynamic‑programming algorithm that scans rows while
 *            keeping the last two rows in memory
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Pre‑compute all admissible 6‑bit row masks and a table that tells
       when three rows are mutually compatible (no column has more than
       one "1" in any three consecutive rows). */
    static int valid[13];
    static int mask[13];
    static unsigned char compat[13][13][13];
    static int init = 0;

    if (!init) {
        int cnt = 0;
        for (int m = 0; m < 64; ++m) {
            int ok = 1;
            for (int j = 0; j <= 3; ++j) {
                int s = ((m >> j) & 1) + ((m >> (j + 1)) & 1) + ((m >> (j + 2)) & 1);
                if (s > 1) { ok = 0; break; }
            }
            if (ok) {
                mask[cnt] = m;
                valid[cnt] = 1;
                ++cnt;
            }
        }
        for (int i = 0; i < cnt; ++i)
            for (int j = 0; j < cnt; ++j)
                for (int k = 0; k < cnt; ++k)
                    compat[i][j][k] = ((mask[i] & mask[j]) == 0 &&
                                      (mask[i] & mask[k]) == 0 &&
                                      (mask[j] & mask[k]) == 0);
        init = 1;
    }

    /* DP over rows: dp[p2][p1] = number of ways to have processed
       some number of rows ending with rows p2 and p1 (p2 = row i‑2,
       p1 = row i‑1).  After processing all rows, the answer is the
       sum of all dp entries. */
    seq_t dp[13][13] = {0};
    seq_t nxt[13][13] = {0};
    dp[0][0] = 1;                      /* starting with two dummy zero rows */

    idx_t rows = n + 2;                 /* total rows to process */
    for (idx_t step = 0; step < rows; ++step) {
        /* reset next DP table */
        for (int a = 0; a < 13; ++a)
            for (int b = 0; b < 13; ++b)
                nxt[a][b] = 0;

        for (int p2 = 0; p2 < 13; ++p2) {
            for (int p1 = 0; p1 < 13; ++p1) {
                seq_t ways = dp[p2][p1];
                if (ways == 0) continue;
                for (int r = 0; r < 13; ++r) {
                    if (!compat[p2][p1][r]) continue;
                    nxt[p1][r] += ways;
                }
            }
        }

        /* copy next to dp for next iteration */
        for (int a = 0; a < 13; ++a)
            for (int b = 0; b < 13; ++b)
                dp[a][b] = nxt[a][b];
    }

    /* Sum all possibilities after all rows processed */
    seq_t result = 0;
    for (int a = 0; a < 13; ++a)
        for (int b = 0; b < 13; ++b)
            result += dp[a][b];

    return result;
}
