/*
 * A204194  Number of nX5 0..1 arrays with no occurrence of three equal
 *          elements in a row horizontally, vertically or nw-to-se diagonally,
 *          and new values 0..1 introduced in row major order.
 * https://oeis.org/A204194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ½ × (number of n×5 binary arrays with no three identical
 *            elements in a horizontal, vertical or NW‑SE diagonal contiguous
 *            block). Computation is done by dynamic programming over all
 *            valid rows
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pre‑generated: all 5‑bit rows that satisfy the horizontal rule
       (no three consecutive identical bits).  There are 16 such rows. */
    static const uint8_t valid_rows[16] = {
        0b00100, 0b00101, 0b00110, 0b01001, 0b01010, 0b01011,
        0b01100, 0b01101, 0b10010, 0b10011, 0b10100, 0b10101,
        0b10110, 0b11001, 0b11010, 0b11011
    };
    const int R = 16;                /* number of valid rows  */
    const int STATE = R * R;         /* pair of consecutive rows */

    /* Transition tables: for each pair (prev2,prev1) the list of new rows
       that keep the vertical and diagonal constraints satisfied.  */
    static int trans_cnt[16][16];
    static uint8_t trans_new[16][16][16];
    static int trans_ready = 0;

    /* Build transition tables only once */
    if (!trans_ready) {
        for (int i = 0; i < R; ++i) {
            for (int j = 0; j < R; ++j) {
                int cnt = 0;
                uint8_t r1 = valid_rows[i];
                uint8_t r2 = valid_rows[j];
                for (int k = 0; k < R; ++k) {
                    uint8_t r3 = valid_rows[k];

                    /* vertical check: no column with three equal bits */
                    int vert_ok = 1;
                    for (int c = 0; c < 5 && vert_ok; ++c) {
                        int b1 = (r1 >> c) & 1;
                        int b2 = (r2 >> c) & 1;
                        int b3 = (r3 >> c) & 1;
                        if (b1 == b2 && b2 == b3)
                            vert_ok = 0;
                    }
                    if (!vert_ok) continue;

                    /* diagonal NW‑SE check: no 3‑cell diagonal with equal bits */
                    int diag_ok = 1;
                    for (int c = 2; c < 5 && diag_ok; ++c) {
                        int b1 = (r1 >> (c - 2)) & 1;
                        int b2 = (r2 >> (c - 1)) & 1;
                        int b3 = (r3 >> c) & 1;
                        if (b1 == b2 && b2 == b3)
                            diag_ok = 0;
                    }
                    if (!diag_ok) continue;

                    /* transition allowed */
                    trans_new[i][j][cnt++] = k;
                }
                trans_cnt[i][j] = cnt;
            }
        }
        trans_ready = 1;
    }

    /* Special cases for n = 0 or 1 */
    if (n == 0)
        return 0;
    if (n == 1) {
        seq_t total = (seq_t)R;     /* total of 1‑row arrays */
        return total / 2;           /* only those starting with 0 are counted */
    }

    /* DP arrays for the last two rows */
    seq_t dp[STATE];
    seq_t ndp[STATE];
    for (int i = 0; i < STATE; ++i) dp[i] = 0;

    /* Initialize for the first two rows: all pairs are allowed */
    for (int i = 0; i < R; ++i)
        for (int j = 0; j < R; ++j)
            dp[i * R + j] = 1;

    /* Process remaining rows */
    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < STATE; ++i) ndp[i] = 0;
        for (int id = 0; id < STATE; ++id) {
            if (dp[id]) {
                int prev2 = id / R;
                int prev1 = id % R;
                int cnt = trans_cnt[prev2][prev1];
                seq_t val = dp[id];
                for (int t = 0; t < cnt; ++t) {
                    int newrow = trans_new[prev2][prev1][t];
                    int nid = prev1 * R + newrow;
                    ndp[nid] += val;
                }
            }
        }
        /* swap dp and ndp */
        for (int i = 0; i < STATE; ++i)
            dp[i] = ndp[i];
    }

    /* Sum all states to get total number of arrays */
    seq_t total = 0;
    for (int i = 0; i < STATE; ++i)
        total += dp[i];

    /* Only arrays that start with 0 are counted */
    return total / 2;
}
