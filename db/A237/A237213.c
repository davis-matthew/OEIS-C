/*
 * A237213  Number of (n+1)X(2+1) 0..3 arrays with the maximum plus the upper
 *          median minus the lower median minus the minimum of every 2X2
 *          subblock equal.
 * https://oeis.org/A237213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   _calc_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A237213:
 * Number of (n+1)×3 arrays with entries 0..3 such that
 * for every 2×2 subblock the value
 *   max + second‑max – second‑min – min
 * is the same for all subblocks.
 *
 * Implementation: dynamic programming on the directed graph of 64
 * possible rows of length 3.  For each possible value c∈{0,…,6}
 * we pre‑compute all admissible edges (row → next row) for which
 * both 2×2 subblocks have value c.  The number of arrays of size
 * (n+1)×3 is the number of walks of length n in any of the 7
 * sub‑graphs, summed over all c.  The algorithm runs in O(7·64²·n)
 * time and uses only O(64) memory per graph.  No sequences are
 * hard‑coded; the result is computed entirely from the definition.
 */

#include <stdbool.h>
#include <string.h>
#include "../utils/_calc_s.h"

/* adjacency information for each constant c (0…6) */
static int _initialized = 0;
static int _adj_cnt[7][64];      /* number of outgoing edges for a given row */
static int _adj_to[7][64][64];   /* adjacency lists: _adj_to[c][row][k] = next_row */

/* build adjacency lists for all constants 0…6 */
static void _init_adj(void)
{
    for (int c = 0; c < 7; ++c)
        for (int r = 0; r < 64; ++r)
            _adj_cnt[c][r] = 0;

    for (int r1 = 0; r1 < 64; ++r1) {
        int a0 = (r1 >> 4) & 3;
        int a1 = (r1 >> 2) & 3;
        int a2 = r1 & 3;
        for (int r2 = 0; r2 < 64; ++r2) {
            int b0 = (r2 >> 4) & 3;
            int b1 = (r2 >> 2) & 3;
            int b2 = r2 & 3;

            int s0 = _calc_s(a0, a1, b0, b1);   /* left 2×2 */
            int s1 = _calc_s(a1, a2, b1, b2);   /* right 2×2 */

            if (s0 == s1) {
                int c = s0;        /* 0…6 */
                int idx = _adj_cnt[c][r1]++;
                _adj_to[c][r1][idx] = r2;
            }
        }
    }
    _initialized = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence starts at n = 1 */
    if (!_initialized) _init_adj();

    seq_t result = 0;

    for (int c = 0; c < 7; ++c) {           /* iterate over all possible constant values */
        seq_t dp[64];
        seq_t ndp[64];

        /* any first row is allowed */
        for (int i = 0; i < 64; ++i) dp[i] = 1;

        for (int step = 0; step < (int)n; ++step) {
            memset(ndp, 0, sizeof(ndp));
            for (int i = 0; i < 64; ++i) {
                seq_t val = dp[i];
                if (!val) continue;
                int *dest = _adj_to[c][i];
                int cnt = _adj_cnt[c][i];
                for (int k = 0; k < cnt; ++k) {
                    ndp[dest[k]] += val;
                }
            }
            for (int i = 0; i < 64; ++i) dp[i] = ndp[i];
        }

        for (int i = 0; i < 64; ++i) result += dp[i];
    }

    return result;
}
