/*
 * A207072  Number of 5Xn 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n 0–1 arrays avoiding 0 0 1 and 0 1 0
 *            horizontally and 0 0 1 and 1 0 1 vertically. Computed by dynamic
 *            programming over column patterns
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 36, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Allow column patterns (5-bit).  Precomputation is done once. */
    static bool init = false;
    static int  m = 0;                 /* number of allowed columns      */
    static int  allow[32];             /* allowed column bitmasks        */
    static int  trans[32][32][32];     /* transitions (i,j) → k indices */
    static int  transCount[32][32];    /* number of transitions per (i,j) */

    if (!init) {
        /* Enumerate 5‑bit columns and filter vertical patterns. */
        int pos = 0;
        for (int col = 0; col < 32; ++col) {
            bool ok = true;
            for (int r = 0; r < 3; ++r) {
                int a = (col >> r)       & 1;
                int b = (col >> (r + 1)) & 1;
                int c = (col >> (r + 2)) & 1;
                if ((a == 0 && b == 0 && c == 1) ||
                    (a == 1 && b == 0 && c == 1)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                allow[pos++] = col;
            }
        }
        m = pos;

        /* Build transition lists: for each pair (i,j) find k
           such that the triple (i,j,k) satisfies the horizontal rule. */
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                int cnt = 0;
                for (int k = 0; k < m; ++k) {
                    bool ok = true;
                    for (int r = 0; r < 5; ++r) {
                        int x = (allow[i] >> r) & 1;
                        int y = (allow[j] >> r) & 1;
                        int z = (allow[k] >> r) & 1;
                        if ((x == 0 && y == 0 && z == 1) ||
                            (x == 0 && y == 1 && z == 0)) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) {
                        trans[i][j][cnt++] = k;
                    }
                }
                transCount[i][j] = cnt;
            }
        }
        init = true;
    }

    /* Handle small indices directly. */
    if (n == 0) return 0;          /* undefined by the offset, but safe */
    if (n == 1) return (seq_t)m;   /* one column, any allowed pattern */
    if (n == 2) return (seq_t)m * m; /* two columns, no horizontal rule */

    /* Dynamic programming: dp[i][j] = number of ways ending with columns
       i (second last) and j (last).  Work on integers from 0 to m-1. */
    seq_t dp[32][32] = {0};
    seq_t ndp[32][32];

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            dp[i][j] = 1;  /* base for n = 2  */
        }
    }

    for (idx_t step = 3; step <= n; ++step) {
        /* clear ndp */
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                ndp[i][j] = 0;
            }
        }

        /* propagate */
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                seq_t val = dp[i][j];
                if (val == 0) continue;
                int cnt = transCount[i][j];
                for (int t = 0; t < cnt; ++t) {
                    int k = trans[i][j][t];
                    ndp[j][k] += val;
                }
            }
        }

        /* copy ndp into dp for the next step */
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                dp[i][j] = ndp[i][j];
            }
        }
    }

    /* Sum all ways for the final column pair. */
    seq_t total = 0;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            total += dp[i][j];
        }
    }
    return total;
}
