/*
 * A219082  Number of 5 X n binary arrays with every 0 a horizontal, diagonal
 *          or antidiagonal neighbor to some 1.
 * https://oeis.org/A219082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Count 5×n binary matrices where each 0 has a 1 among its horizontal,
   diagonal, or antidiagonal neighbors (OEIS A219082).  Uses a transfer‑matrix DP
   over column patterns.  State: previous column pattern and the set of rows
   with an unsatisfied 0 that must be satisfied by the next column. */

#include <stdint.h>
#include <strings.h>   /* for memset (may use <string.h> in C++ as well) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; we never get n=0 in tests */

    /* Tables that depend only on the pattern alphabet (5 rows: 32 patterns). */
    static unsigned char rowMask[5];
    static unsigned char unsatCur[32][32];
    static unsigned char nextLists[32][32];
    static unsigned char nextCount[32];
    static int initialized = 0;

    if (!initialized) {
        /* rowMask[r] = bits of rows that can satisfy a 0 at row r from a neighboring column */
        for (int r = 0; r < 5; ++r) {
            unsigned m = 1u << r;
            if (r > 0) m |= 1u << (r - 1);
            if (r < 4) m |= 1u << (r + 1);
            rowMask[r] = (unsigned char)m;
        }

        /* unsatCur[prev][cur] : rows that remain unsatisfied after choosing cur
           given that left neighbor is prev */
        for (int p = 0; p < 32; ++p) {
            for (int c = 0; c < 32; ++c) {
                unsigned char uns = 0;
                for (int r = 0; r < 5; ++r) {
                    if ((c & (1u << r)) == 0) {          /* zero in current column */
                        if ((p & rowMask[r]) == 0) {      /* left neighbor does not satisfy */
                            uns |= 1u << r;
                        }
                    }
                }
                unsatCur[p][c] = uns;
            }
        }

        /* nextLists[u] = list of patterns that satisfy all unsatisfied rows u */
        for (int u = 0; u < 32; ++u) {
            int cnt = 0;
            for (int c = 0; c < 32; ++c) {
                int ok = 1;
                for (int r = 0; r < 5 && ok; ++r) {
                    if (u & (1u << r)) {
                        if ((c & rowMask[r]) == 0) ok = 0;
                    }
                }
                if (ok) nextLists[u][cnt++] = (unsigned char)c;
            }
            nextCount[u] = cnt;
        }
        initialized = 1;
    }

    /* DP tables: dp[cur_pattern][unsatisfied_mask] */
    static seq_t dp_cur[32][32];
    static seq_t dp_next[32][32];

    /* clear dp_cur */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp_cur[i][j] = 0;

    /* initial column (column 1): no left neighbor, so every 0 is unsatisfied */
    for (int p = 0; p < 32; ++p) {
        unsigned char uns = (~p) & 0x1Fu;    /* bits where p has 0 */
        dp_cur[p][uns] = 1;
    }

    /* Process columns 2 .. n */
    for (idx_t col = 2; col <= n; ++col) {
        /* clear dp_next */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp_next[i][j] = 0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int uns = 0; uns < 32; ++uns) {
                seq_t val = dp_cur[prev][uns];
                if (val == 0) continue;
                int cnt = nextCount[uns];
                for (int k = 0; k < cnt; ++k) {
                    int cur = nextLists[uns][k];
                    unsigned char unsNew = unsatCur[prev][cur];
                    dp_next[cur][unsNew] += val;
                }
            }
        }

        /* swap dp_cur and dp_next */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp_cur[i][j] = dp_next[i][j];
    }

    /* Sum all states with no unsatisfied rows after the last column */
    seq_t result = 0;
    for (int p = 0; p < 32; ++p)
        result += dp_cur[p][0];

    return result;
}
