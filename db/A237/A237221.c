/*
 * A237221  Number of (n+1)X(3+1) 0..3 arrays with the maximum plus the lower
 *          median minus the upper median of every 2X2 subblock equal.
 * https://oeis.org/A237221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays over {0,1,2,3} where for every
 *            2×2 subblock the value max + lower median - upper median is the
 *            same for all subblocks. Computed by dynamic programming over the
 *            rows
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   block_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/block_k.h"

static int initialized = 0;
static uint8_t row_d[256][4];
static uint16_t neigh[4][256][256];
static uint16_t neigh_len[4][256];

static void init(void)
{
    if (initialized) return;
    initialized = 1;

    /* pre‑compute row digit patterns */
    for (int r = 0; r < 256; ++r) {
        row_d[r][0] = r & 3;
        row_d[r][1] = (r >> 2) & 3;
        row_d[r][2] = (r >> 4) & 3;
        row_d[r][3] = (r >> 6) & 3;
    }

    /* build adjacency lists per K value */
    for (int K = 0; K < 4; ++K)
        for (int r = 0; r < 256; ++r)
            neigh_len[K][r] = 0;

    for (int r = 0; r < 256; ++r) {
        for (int s = 0; s < 256; ++s) {
            int k0 = block_k(row_d[r][0], row_d[r][1], row_d[s][0], row_d[s][1]);
            int k1 = block_k(row_d[r][1], row_d[r][2], row_d[s][1], row_d[s][2]);
            int k2 = block_k(row_d[r][2], row_d[r][3], row_d[s][2], row_d[s][3]);

            if (k0 == k1 && k1 == k2) {
                uint8_t K = (uint8_t)k0;
                neigh[K][r][neigh_len[K][r]++] = (uint16_t)s;
            }
        }
    }
}

seq_t a(idx_t n)
{
    init();

    seq_t result = 0;
    idx_t steps = n;                 /* number of transitions: (n+1) rows → n steps */

    for (uint8_t K = 0; K < 4; ++K) {
        seq_t cur[256], nxt[256];

        for (int i = 0; i < 256; ++i)
            cur[i] = (seq_t)1;       /* all rows are allowed as start */

        for (idx_t step = 0; step < steps; ++step) {
            for (int i = 0; i < 256; ++i)
                nxt[i] = (seq_t)0;

            for (int r = 0; r < 256; ++r) {
                seq_t val = cur[r];
                if (val == (seq_t)0) continue;

                uint16_t len = neigh_len[K][r];
                uint16_t *neighs = neigh[K][r];
                for (uint16_t idx = 0; idx < len; ++idx) {
                    int s = neighs[idx];
                    nxt[s] += val;
                }
            }

            /* move to next state */
            for (int i = 0; i < 256; ++i)
                cur[i] = nxt[i];
        }

        /* accumulate results for this K */
        for (int i = 0; i < 256; ++i)
            result += cur[i];
    }

    return result;
}
