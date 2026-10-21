/*
 * A237204  Number of (n+1)X(1+1) 0..3 arrays with the upper median plus the
 *          lower median minus the minimum of every 2X2 subblock equal.
 * https://oeis.org/A237204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   value_for_states
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) counts (n+1)x2 arrays over {0,1,2,3} such that for every 2×2 block
   the upper median plus the lower median minus the minimum is constant.
   The value is computed by summing, over all possible constants K (0..6),
   the total number of length‑n walks in the 16‑state graphs G_K where
   an edge (i,j) exists iff that expression equals K. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/value_for_states.h"

static bool ADJ[7][16][16] = {0};
static bool ADJ_READY = false;

static void build_adj(void)
{
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            int k = value_for_states(i, j);
            ADJ[k][i][j] = true;
        }
    }
    ADJ_READY = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1 – irrelevant for OEIS */
    if (!ADJ_READY) build_adj();

    seq_t result = 0;

    /* iterate over all possible constants K (0..6) */
    for (int K = 0; K <= 6; ++K) {
        seq_t dp_cur[16];
        seq_t dp_nxt[16];

        /* start with any first row */
        for (int s = 0; s < 16; ++s) dp_cur[s] = 1;

        /* perform n transitions (there are n subblocks) */
        for (idx_t step = 0; step < n; ++step) {
            for (int j = 0; j < 16; ++j) dp_nxt[j] = 0;
            for (int i = 0; i < 16; ++i) {
                seq_t val = dp_cur[i];
                if (!val) continue;
                for (int j = 0; j < 16; ++j)
                    if (ADJ[K][i][j])
                        dp_nxt[j] += val;
            }
            for (int j = 0; j < 16; ++j) dp_cur[j] = dp_nxt[j];
        }

        /* sum all ending states: number of length‑(n+1) arrays for this K */
        seq_t sumK = 0;
        for (int s = 0; s < 16; ++s) sumK += dp_cur[s];
        result += sumK;
    }
    return result;
}
