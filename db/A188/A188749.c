/*
 * A188749  Number of 4 X n binary arrays without the pattern 1 1 1
 *          diagonally, vertically or antidiagonally.
 * https://oeis.org/A188749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: DP over columns with state of two previous 4‑bit column masks
   counting 4×n binary arrays that avoid a 111 pattern diagonally, vertically,
   or anti‑diagonally. */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */
    const int MASKS = 1 << 4;      /* 4 rows → 16 masks */

    /* ---- precompute legal column masks (no vertical 111) ---- */
    static uint8_t legal[16];
    static int legal_cnt = -1;
    if (legal_cnt < 0) {
        legal_cnt = 0;
        for (int m = 0; m < MASKS; ++m) {
            int v1 = (m & 0b0111) == 0b0111;   /* rows 0‑2 all 1 */
            int v2 = (m & 0b1110) == 0b1110;   /* rows 1‑3 all 1 */
            if (!(v1 || v2)) {
                legal[legal_cnt++] = (uint8_t)m;
            }
        }
    }

    /* ---- handle n==1, n==2 directly ---- */
    if (n == 1) {
        seq_t r = 0;
        for (int i = 0; i < legal_cnt; ++i) r += 1;
        return r;
    }
    if (n == 2) {
        seq_t r = 0;
        for (int i = 0; i < legal_cnt; ++i)
            for (int j = 0; j < legal_cnt; ++j) r += 1;
        return r;
    }

    /* ---- dynamic programming for n >= 3 ---- */
    seq_t dp[16][16];
    seq_t dp_next[16][16];

    /* initialise dp for 2 columns: state = (col1, col2) */
    for (int i = 0; i < MASKS; ++i)
        for (int j = 0; j < MASKS; ++j)
            dp[i][j] = 0;
    for (int i = 0; i < legal_cnt; ++i) {
        int a = legal[i];
        for (int j = 0; j < legal_cnt; ++j) {
            int b = legal[j];
            dp[a][b] = 1;
        }
    }

    /* sum of dp entries gives count for current column number */
    seq_t ans = 0;
    for (int i = 0; i < MASKS; ++i)
        for (int j = 0; j < MASKS; ++j)
            ans += dp[i][j];

    /* iterate columns 3 .. n */
    for (idx_t col = 3; col <= n; ++col) {
        /* clear dp_next */
        for (int i = 0; i < MASKS; ++i)
            for (int j = 0; j < MASKS; ++j)
                dp_next[i][j] = 0;

        /* transition: from state (a,b) add new column c */
        for (int a = 0; a < MASKS; ++a) {
            for (int b = 0; b < MASKS; ++b) {
                seq_t count = dp[a][b];
                if (count == 0) continue;
                for (int ci = 0; ci < legal_cnt; ++ci) {
                    int c = legal[ci];
                    /* main diagonal: (a[i], b[i+1], c[i+2]) for i=0,1 */
                    if ((a & 0b0001) && (b & 0b0010) && (c & 0b0100)) continue;
                    if ((a & 0b0010) && (b & 0b0100) && (c & 0b1000)) continue;
                    /* anti‑diagonal: (a[i+2], b[i+1], c[i]) for i=0,1 */
                    if ((a & 0b0100) && (b & 0b0010) && (c & 0b0001)) continue;
                    if ((a & 0b1000) && (b & 0b0100) && (c & 0b0010)) continue;
                    dp_next[b][c] += count;
                }
            }
        }

        /* copy dp_next to dp */
        for (int i = 0; i < MASKS; ++i)
            for (int j = 0; j < MASKS; ++j)
                dp[i][j] = dp_next[i][j];

        /* compute new ans */
        ans = 0;
        for (int i = 0; i < MASKS; ++i)
            for (int j = 0; j < MASKS; ++j)
                ans += dp[i][j];
    }

    return ans;
}
