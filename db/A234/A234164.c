/*
 * A234164  Number of (n+1) X (3+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   delta_idx, weight_from_delta
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A234164: Number of (n+1)×4 arrays with entries 0..4
   such that every 2×2 subblock has its diagonal sum differing
   from its antidiagonal sum by 2. */
#include <stddef.h>
#include "../utils/delta_idx.h"
#include "../utils/weight_from_delta.h"

#define MAX_STATES 729       /* 9^3 possible difference triples */

static int w[MAX_STATES];                      /* weight (number of offsets) for each state   */
static int trans_to[MAX_STATES][8];             /* destination indices (max 8 per state)       */
static int trans_cnt[MAX_STATES];               /* number of transitions for each state       */
static int initialized = 0;

seq_t a(idx_t n)
{
    /* one‑time initialization of states and transitions */
    if (!initialized) {
        /* compute weight for all states */
        for (int d0 = -4; d0 <= 4; ++d0)
            for (int d1 = -4; d1 <= 4; ++d1)
                for (int d2 = -4; d2 <= 4; ++d2) {
                    int idx = delta_idx(d0, d1, d2);
                    w[idx] = weight_from_delta(d0, d1, d2);
                    trans_cnt[idx] = 0;
                }

        /* compute transitions: each component changes by ±2 */
        for (int d0 = -4; d0 <= 4; ++d0)
            for (int d1 = -4; d1 <= 4; ++d1)
                for (int d2 = -4; d2 <= 4; ++d2) {
                    int src = delta_idx(d0, d1, d2);
                    if (w[src] == 0) continue;
                    int ds[3] = {d0, d1, d2};
                    for (int s0 = -1; s0 <= 1; s0 += 2)
                        for (int s1 = -1; s1 <= 1; s1 += 2)
                            for (int s2 = -1; s2 <= 1; s2 += 2) {
                                int nd0 = ds[0] + 2 * s0;
                                int nd1 = ds[1] + 2 * s1;
                                int nd2 = ds[2] + 2 * s2;
                                if (nd0 < -4 || nd0 > 4 ||
                                    nd1 < -4 || nd1 > 4 ||
                                    nd2 < -4 || nd2 > 4) continue;
                                int dst = delta_idx(nd0, nd1, nd2);
                                if (w[dst] == 0) continue;   /* no rows for this state */
                                int c = trans_cnt[src];
                                trans_to[src][c] = dst;
                                trans_cnt[src] = c + 1;
                            }
                }
        initialized = 1;
    }

    /* DP arrays */
    seq_t dp[MAX_STATES];
    seq_t nextdp[MAX_STATES];

    /* initialise dp with weight of first row */
    for (int i = 0; i < MAX_STATES; ++i)
        dp[i] = (w[i] == 0) ? 0 : (seq_t)w[i];

    /* perform n transitions – each transition adds one more row */
    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < MAX_STATES; ++i)
            nextdp[i] = 0;
        for (int src = 0; src < MAX_STATES; ++src) {
            seq_t val = dp[src];
            if (val == 0) continue;
            int c = trans_cnt[src];
            int *dest = trans_to[src];
            for (int j = 0; j < c; ++j) {
                int dst = dest[j];
                int wdst = w[dst];
                for (int k = 0; k < wdst; ++k)
                    nextdp[dst] += val;
            }
        }
        for (int i = 0; i < MAX_STATES; ++i)
            dp[i] = nextdp[i];
    }

    /* sum over all final states */
    seq_t res = 0;
    for (int i = 0; i < MAX_STATES; ++i)
        res += dp[i];
    return res;
}
