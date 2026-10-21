/*
 * A237967  Number of (n+1)X(1+1) 0..3 arrays with the upper median of every
 *          2X2 subblock differing from its horizontal and vertical neighbors
 *          by exactly one.
 * https://oeis.org/A237967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×2 arrays of 0..3 such that for every 2×2
 *            subblock the upper median of that subblock differs from the
 *            upper median of its vertically neighboring subblock by exactly
 *            1. Computed by dynamic programming over block medians
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   UpperMedian4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/UpperMedian4.h"

/* Transition tables: M[s][t] = upper median of block formed by rows s and t
   (rows are encoded as 0..15 with state=4*col0+col1).
   adj[s][t] = 1 if state s can transition to state t
   (i.e., the previous and next block medians differ by exactly 1). */

static int M[16][16];
static char adj[64][64];
static bool adj_inited = false;

static void init_transition(void)
{
    if (adj_inited) return;

    /* Precompute M table. */
    for (int a = 0; a < 4; ++a) {
        for (int b = 0; b < 4; ++b) {
            for (int c = 0; c < 4; ++c) {
                for (int d = 0; d < 4; ++d) {
                    int s = 4 * a + b;
                    int t = 4 * c + d;
                    M[s][t] = UpperMedian4(a, b, c, d);
                }
            }
        }
    }

    /* Build adjacency on 64 states (row value + previous block median). */
    for (int row = 0; row < 16; ++row) {
        for (int pm = 0; pm < 4; ++pm) {
            int spos = 4 * row + pm;
            for (int next = 0; next < 16; ++next) {
                int mn = M[row][next];
                if ((pm + 1 == mn) || (pm - 1 == mn)) {
                    int tpos = 4 * next + mn;
                    adj[spos][tpos] = 1;
                }
            }
        }
    }
    adj_inited = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used, but defined for completeness */

    init_transition();

    seq_t dp[64] = {0};
    seq_t ndp[64];

    /* Initial state after the first 2×2 block (rows 0 and 1). */
    for (int r0 = 0; r0 < 16; ++r0) {
        for (int r1 = 0; r1 < 16; ++r1) {
            int m = M[r0][r1];
            int idx = 4 * r1 + m;
            dp[idx] += (seq_t)1;
        }
    }

    /* Each additional row adds one more block.  We need (n‑1) transitions. */
    for (idx_t step = 1; step < n; ++step) {
        for (int i = 0; i < 64; ++i) ndp[i] = (seq_t)0;
        for (int s = 0; s < 64; ++s) {
            seq_t val = dp[s];
            if (val == (seq_t)0) continue;
            for (int t = 0; t < 64; ++t) {
                if (adj[s][t]) ndp[t] += val;
            }
        }
        for (int i = 0; i < 64; ++i) dp[i] = ndp[i];
    }

    /* Sum all possible final states. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 64; ++i) total += dp[i];
    return total;
}
