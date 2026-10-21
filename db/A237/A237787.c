/*
 * A237787  Number of (n+1)X(2+1) 0..3 arrays with the maximum plus the
 *          minimum minus the lower median of every 2X2 subblock differing
 *          from its horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A237787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays over {0,1,2,3} such that for
 *            every 2×2 subblock let V = max+min−lower_median. All
 *            horizontally or vertically adjacent V values differ by exactly
 *            one. Computed by a simple dynamic program over rows
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   adj1, fvalue_four
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/adj1.h"
#include "../utils/fvalue_four.h"

/* dimensions of a row: 3 columns, each entry 0..3 */
#define ROWS 3
#define COLS 3
#define MAX_ROW_VALUE 4
#define V_STATE 4            /* possible V values 0..3 */

/* pre‑computed digit extraction for each row code 0..63 */
static uint8_t row_dig[64][ROWS];
static bool row_dig_done = false;
static void init_row_digits(void)
{
    if (row_dig_done) return;
    for (int code = 0; code < 64; ++code) {
        int x = code;
        for (int k = 0; k < ROWS; ++k) {
            row_dig[code][k] = x & 3;
            x >>= 2;
        }
    }
    row_dig_done = true;
}

/* pre‑computed V values for all ordered row pairs */
static uint8_t block_f[64][64][2];
static bool block_f_done = false;
static void init_block_f(void)
{
    if (block_f_done) return;
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 64; ++j) {
            /* block columns 0–1 */
            uint8_t v0 = fvalue_four(row_dig[i][0], row_dig[i][1],
                                     row_dig[j][0], row_dig[j][1]);
            block_f[i][j][0] = v0;
            /* block columns 1–2 */
            uint8_t v1 = fvalue_four(row_dig[i][1], row_dig[i][2],
                                     row_dig[j][1], row_dig[j][2]);
            block_f[i][j][1] = v1;
        }
    }
    block_f_done = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not defined for offset 1, but safe */

    init_row_digits();
    init_block_f();

    /* dp arrays: dp_prev[row][v0][v1] */
    static seq_t dp_prev[64][V_STATE][V_STATE];
    static seq_t dp_next[64][V_STATE][V_STATE];

    /* zero arrays */
    for (int r = 0; r < 64; ++r)
        for (int v0 = 0; v0 < V_STATE; ++v0)
            for (int v1 = 0; v1 < V_STATE; ++v1)
                dp_prev[r][v0][v1] = 0;

    /* initial step: one subblock between row0 and row1 */
    for (int r0 = 0; r0 < 64; ++r0) {
        for (int r1 = 0; r1 < 64; ++r1) {
            uint8_t v0 = block_f[r0][r1][0];
            uint8_t v1 = block_f[r0][r1][1];
            if (adj1(v0, v1)) {
                dp_prev[r1][v0][v1] += (seq_t)1;
            }
        }
    }

    /* if only one subblock, return the total */
    if (n == 1) {
        seq_t total = 0;
        for (int r = 0; r < 64; ++r)
            for (int v0 = 0; v0 < V_STATE; ++v0)
                for (int v1 = 0; v1 < V_STATE; ++v1)
                    total += dp_prev[r][v0][v1];
        return total;
    }

    /* process remaining subblocks */
    for (idx_t step = 1; step < n; ++step) {   /* already processed 1 */
        /* zero next dp */
        for (int r = 0; r < 64; ++r)
            for (int v0 = 0; v0 < V_STATE; ++v0)
                for (int v1 = 0; v1 < V_STATE; ++v1)
                    dp_next[r][v0][v1] = 0;

        for (int prv = 0; prv < 64; ++prv) {
            for (int bv0 = 0; bv0 < V_STATE; ++bv0) {
                for (int bv1 = 0; bv1 < V_STATE; ++bv1) {
                    seq_t cnt = dp_prev[prv][bv0][bv1];
                    if (cnt == 0) continue;
                    for (int nxt = 0; nxt < 64; ++nxt) {
                        uint8_t nv0 = block_f[prv][nxt][0];
                        uint8_t nv1 = block_f[prv][nxt][1];
                        /* horizontal constraint */
                        if (!adj1(nv0, nv1)) continue;
                        /* vertical constraints */
                        if (!adj1(nv0, bv0)) continue;
                        if (!adj1(nv1, bv1)) continue;
                        dp_next[nxt][nv0][nv1] += cnt;
                    }
                }
            }
        }

        /* copy dp_next back to dp_prev for next iteration */
        for (int r = 0; r < 64; ++r)
            for (int v0 = 0; v0 < V_STATE; ++v0)
                for (int v1 = 0; v1 < V_STATE; ++v1)
                    dp_prev[r][v0][v1] = dp_next[r][v0][v1];
    }

    /* sum final counts */
    seq_t total = 0;
    for (int r = 0; r < 64; ++r)
        for (int v0 = 0; v0 < V_STATE; ++v0)
            for (int v1 = 0; v1 < V_STATE; ++v1)
                total += dp_prev[r][v0][v1];
    return total;
}
