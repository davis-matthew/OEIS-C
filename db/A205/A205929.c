/*
 * A205929  Number of (n+1)X3 0..3 arrays with the number of clockwise edge
 *          increases in 2X2 subblocks nondecreasing rightwards and downwards.
 * https://oeis.org/A205929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays filled with 0..3 such that * for
 *            each 2×2 subblock the number of clockwise edge increases is *
 *            nondecreasing towards the right and downwards. (OEIS A205929)
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1 – behaviour below 1 is unused */

    /* ---- precomputation of row encodings and edge increase data ---- */
    static int  row_val[64][3];
    static unsigned char e_tbl[64][64][2];
    static int  init = 0;

    if (!init) {
        /* encode every 3‑tuple in base‑4 */
        for (int idx = 0; idx < 64; ++idx)
        {
            int x = idx;
            row_val[idx][2] = x & 3;  x >>= 2;
            row_val[idx][1] = x & 3;  x >>= 2;
            row_val[idx][0] = x & 3;
        }

        /* precompute the two edge‐increase counts for every ordered pair
           of rows.  Edge 0 uses columns 0‑1, edge 1 uses columns 1‑2. */
        for (int i = 0; i < 64; ++i)
        {
            for (int j = 0; j < 64; ++j)
            {
                for (int col0 = 0; col0 < 2; ++col0)
                {
                    int a = row_val[i][col0];
                    int b = row_val[i][col0+1];
                    int c = row_val[j][col0];
                    int d = row_val[j][col0+1];
                    int inc = 0;
                    if (a < b) ++inc;          /* top edge   */
                    if (b < d) ++inc;          /* right edge */
                    if (d < c) ++inc;          /* bottom edge */
                    if (a > c) ++inc;          /* left  edge */
                    e_tbl[i][j][col0] = (unsigned char)inc;
                }
            }
        }
        init = 1;
    }

    /* DP arrays: dp[64][5][5] – number of ways ending with previous row = r
     * and previous edge counts (e0,e1).  The indices e0,e1 range 0..4.      */
    static seq_t dp[64][5][5];
    static seq_t newdp[64][5][5];

    /* ---- first transition (row 0 → row 1) ---- */
    for (int i = 0; i < 64; ++i)
        for (int a = 0; a < 5; ++a)
            for (int b = 0; b < 5; ++b)
                newdp[i][a][b] = 0;

    for (int prev = 0; prev < 64; ++prev)
        for (int cur  = 0; cur  < 64; ++cur)
        {
            unsigned char e0 = e_tbl[prev][cur][0];
            unsigned char e1 = e_tbl[prev][cur][1];
            if (e0 <= e1)                       /* monotone to the right */
                newdp[cur][e0][e1] += (seq_t)1;
        }

    /* Handle the trivial case n == 1 */
    if (n == 1) {
        seq_t total = 0;
        for (int i = 0; i < 64; ++i)
            for (int a = 0; a < 5; ++a)
                for (int b = 0; b < 5; ++b)
                    total += newdp[i][a][b];
        return total;
    }

    /* Copy the first transition results into dp */
    for (int i = 0; i < 64; ++i)
        for (int a = 0; a < 5; ++a)
            for (int b = 0; b < 5; ++b)
                dp[i][a][b] = newdp[i][a][b];

    /* ---- subsequent transitions (rows 2 … n) ---- */
    for (idx_t step = 2; step <= n; ++step) {
        /* zero the next DP array */
        for (int i = 0; i < 64; ++i)
            for (int a = 0; a < 5; ++a)
                for (int b = 0; b < 5; ++b)
                    newdp[i][a][b] = 0;

        /* For every possible previous row we build a 2‑D prefix sum over
           its (e0,e1) states, only using addition. */
        for (int prev = 0; prev < 64; ++prev) {
            seq_t pref[5][5];
            for (int a = 0; a < 5; ++a) {
                seq_t rowSum = 0;
                for (int b = 0; b < 5; ++b) {
                    rowSum += dp[prev][a][b];
                    if (a > 0)
                        pref[a][b] = pref[a-1][b] + rowSum;
                    else
                        pref[a][b] = rowSum;
                }
            }

            /* Now iterate over all possible current rows */
            for (int cur = 0; cur < 64; ++cur) {
                unsigned char e0 = e_tbl[prev][cur][0];
                unsigned char e1 = e_tbl[prev][cur][1];
                if (e0 <= e1) {                    /* rightward monotonicity */
                    /* Sum of all previous (e0',e1') <= (e0,e1) is the
                       needed prefix value. */
                    newdp[cur][e0][e1] += pref[e0][e1];
                }
            }
        }

        /* Copy newdp back into dp for next iteration */
        for (int i = 0; i < 64; ++i)
            for (int a = 0; a < 5; ++a)
                for (int b = 0; b < 5; ++b)
                    dp[i][a][b] = newdp[i][a][b];
    }

    /* ---- sum all final states to obtain the answer ---- */
    seq_t result = 0;
    for (int i = 0; i < 64; ++i)
        for (int a = 0; a < 5; ++a)
            for (int b = 0; b < 5; ++b)
                result += dp[i][a][b];

    return result;
}
