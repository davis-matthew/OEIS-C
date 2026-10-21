/*
 * A205911  Number of (n+1) X 3 0..2 arrays with every 2 X 2 subblock having
 *          the same number of clockwise edge increases as its horizontal
 *          neighbors and the same number of counterclockwise edge increases
 *          as its vertical neighbors.
 * https://oeis.org/A205911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A205911
    a(n) = number of (n+1)×3 arrays over {0,1,2} satisfying the stated
    clockwise/counter‑clockwise edge‑increase conditions.
    The value is computed by dynamic programming on row patterns. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Pre‑computation (run once) */
    static bool init = false;
    static unsigned char trans_count[27][16];
    static unsigned char trans_next[27][16][27];
    static int row[27][3];

    if (!init) {
        /* all 3‑column rows (3^3 = 27) */
        for (int i = 0; i < 27; ++i) {
            int v = i;
            row[i][0] = v % 3;
            v /= 3;
            row[i][1] = v % 3;
            v /= 3;
            row[i][2] = v;
        }

        /* For every pair of rows (a,b) that satisfy the horizontal
           equal‑clockwise condition, store b in a list indexed by the
           resulting counter‑clockwise pair (ccw0,ccw1). */
        for (int a = 0; a < 27; ++a) {
            for (int b = 0; b < 27; ++b) {
                int a0 = row[a][0], a1 = row[a][1], a2 = row[a][2];
                int b0 = row[b][0], b1 = row[b][1], b2 = row[b][2];

                /* left 2×2 block (columns 0‑1) */
                int cw0  = (a1 > a0) + (b1 > a1) + (b0 > b1) + (a0 > b0);
                int ccw0 = (b0 > a0) + (b1 > b0) + (a1 > b1) + (a0 > a1);

                /* right 2×2 block (columns 1‑2) */
                int cw1  = (a2 > a1) + (b2 > a2) + (b1 > b2) + (a1 > b1);
                int ccw1 = (b1 > a1) + (b2 > b1) + (a2 > b2) + (a1 > a2);

                if (cw0 != cw1)            /* horizontal constraint fails */
                    continue;

                int ccw_idx = (ccw0 << 2) | ccw1;   /* 0 … 15  (ccw0,ccw1 in 0…3) */
                unsigned char *dst = trans_next[a][ccw_idx];
                dst[ trans_count[a][ccw_idx]++ ] = (unsigned char)b;
            }
        }
        init = true;
    }

    /* DP arrays */
    static seq_t dp_cur[27][16];
    static seq_t dp_next[27][16];

    /* initialise dp_cur with arrays of length 2 (n==1) */
    for (int r = 0; r < 27; ++r)
        for (int c = 0; c < 16; ++c)
            dp_cur[r][c] = 0;

    for (int a = 0; a < 27; ++a) {
        for (int cc = 0; cc < 16; ++cc) {
            unsigned char len = trans_count[a][cc];
            for (int k = 0; k < len; ++k) {
                unsigned char b = trans_next[a][cc][k];
                dp_cur[b][cc] += 1;  /* each pair (a,b) contributes one array */
            }
        }
    }

    /* extend the array to n+1 rows: (n-1) additional rows */
    for (idx_t step = 1; step <= n - 1; ++step) {
        /* zero dp_next */
        for (int r = 0; r < 27; ++r)
            for (int c = 0; c < 16; ++c)
                dp_next[r][c] = 0;

        for (int r = 0; r < 27; ++r) {
            for (int cc = 0; cc < 16; ++cc) {
                seq_t cnt = dp_cur[r][cc];
                if (cnt == 0) continue;

                unsigned char len = trans_count[r][cc];
                for (int k = 0; k < len; ++k) {
                    unsigned char nxt = trans_next[r][cc][k];
                    dp_next[nxt][cc] += cnt;   /* same counter‑clockwise pair */
                }
            }
        }

        /* move dp_next back to dp_cur */
        for (int r = 0; r < 27; ++r)
            for (int c = 0; c < 16; ++c)
                dp_cur[r][c] = dp_next[r][c];
    }

    /* sum all valid completions */
    seq_t total = 0;
    for (int r = 0; r < 27; ++r)
        for (int c = 0; c < 16; ++c)
            total += dp_cur[r][c];

    return total;
}
