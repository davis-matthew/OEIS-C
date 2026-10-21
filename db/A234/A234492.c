/*
 * A234492  Number of (n+1) X (2+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A234492: Number of (n+1)×3 arrays with entries 0..5 such that each 2×2 subblock
   has its diagonal sum differing from its anti‑diagonal sum by ±2.
   Computed by a transfer‑matrix dynamic programming over the 216 possible rows.
*/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define STATE_CNT 216   /* 6³ possible rows of width 3 */
#define MAX_NEIGH STATE_CNT

/* ------------------------------------------------------------------- */
/* Pre‑computed adjacency lists for the transfer matrix                */
/* ------------------------------------------------------------------- */
static uint8_t  next_list[STATE_CNT][MAX_NEIGH];
static uint16_t next_cnt  [STATE_CNT];
static bool     adj_inited = false;

/* Build adjacencies only once.                                         */
static void build_adjacency(void)
{
    /* All possible rows: encode as digits (a,b,c) with 0<=digit<=5 */
    int digits[STATE_CNT][3];
    for (int a = 0; a < 6; ++a)
        for (int b = 0; b < 6; ++b)
            for (int c = 0; c < 6; ++c) {
                int id = a*36 + b*6 + c;
                digits[id][0] = a;
                digits[id][1] = b;
                digits[id][2] = c;
            }

    for (int i = 0; i < STATE_CNT; ++i) {
        int a0 = digits[i][0], b0 = digits[i][1], c0 = digits[i][2];
        uint16_t cnt = 0;

        for (int j = 0; j < STATE_CNT; ++j) {
            int a1 = digits[j][0], b1 = digits[j][1], c1 = digits[j][2];

            /* first 2×2 block (cols 0–1) */
            int diff1 = (a0 + b1) - (b0 + a1);
            if (diff1 != 2 && diff1 != -2) continue;

            /* second 2×2 block (cols 1–2) */
            int diff2 = (b0 + c1) - (c0 + b1);
            if (diff2 != 2 && diff2 != -2) continue;

            next_list[i][cnt++] = (uint8_t)j;
        }
        next_cnt[i] = cnt;
    }
    adj_inited = true;
}

/* ------------------------------------------------------------------- */
/* Main sequence function                                               */
/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Lazy conversion of a 64‑bit index to a seq_t index.            */
    if (n == 0) return (seq_t)0;                     /* offset starts at 1 */

    /* Make sure adjacency lists are ready.                           */
    if (!adj_inited) build_adjacency();

    /* DP arrays for current and next row counts.                      */
    static seq_t dp[STATE_CNT];
    static seq_t ndp[STATE_CNT];

    /* Initialise: any first row is allowed.                           */
    for (int i = 0; i < STATE_CNT; ++i) dp[i] = (seq_t)1;

    /* Iterate over n steps (adding rows)                              */
    for (idx_t step = 1; step <= n; ++step) {
        /* zero next array                                            */
        memset(ndp, 0, sizeof(ndp));

        /* transition between states                                 */
        for (int i = 0; i < STATE_CNT; ++i) {
            seq_t cur = dp[i];
            if (cur == (seq_t)0) continue;          /* no ways to reach this state */

            uint16_t cnt = next_cnt[i];
            for (uint16_t k = 0; k < cnt; ++k) {
                int j = next_list[i][k];
                ndp[j] += cur;
            }
        }
        /* copy back to dp for next iteration                        */
        memcpy(dp, ndp, sizeof(dp));
    }

    /* Sum over all final states                                     */
    seq_t total = (seq_t)0;
    for (int i = 0; i < STATE_CNT; ++i) total += dp[i];
    return total;
}
