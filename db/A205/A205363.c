/*
 * A205363  Number of (n+1)X2 0..3 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock the same.
 * https://oeis.org/A205363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:
   Compute the number of (n+1)×2 arrays with entries 0..3 such that
   every 2×2 subblock has the same number of clockwise increases.
   This is done by dynamic programming over the 16 possible rows.
   For each possible count of increases c=0..4 we precompute a 16×16
   adjacency matrix A_c[s][t] which is 1 if a pair of consecutive rows
   s (top) and t (bottom) has exactly c increases.
   We then perform n steps (the number of 2×2 subblocks) multiplying
   a row vector of counts by A_c each time, and finally sum over all
   counts and all ending rows. This yields a(n). */

#include <stdbool.h>

/* Pre‑computed adjacency matrices for the 5 possible counts. */
static struct {
    bool   ready;
    unsigned char mat[5][16][16];
} pre = {false};

static void precompute(void)
{
    if (pre.ready) return;
    for (int s = 0; s < 16; ++s)
        for (int t = 0; t < 16; ++t)
            for (int c = 0; c < 5; ++c)
                pre.mat[c][s][t] = 0;

    for (int s = 0; s < 16; ++s) {
        int xs0 = (s >> 2) & 3; /* top-left */
        int xs1 = s & 3;        /* top-right */
        for (int t = 0; t < 16; ++t) {
            int yt0 = (t >> 2) & 3; /* bottom-left */
            int yt1 = t & 3;        /* bottom-right */

            int inc = 0;
            if (xs0 < xs1) ++inc;      /* top edge */
            if (xs1 < yt1) ++inc;      /* right edge */
            if (yt1 < yt0) ++inc;      /* bottom edge (right→left) */
            if (yt0 < xs0) ++inc;      /* left edge (bottom→top) */
            pre.mat[inc][s][t] = 1;
        }
    }
    pre.ready = true;
}

seq_t a(idx_t n)
{
    precompute();
    if (n == 0) return 0;  /* Not in the OEIS domain, but safe. */

    seq_t dp[5][16];
    seq_t newdp[5][16];

    /* Initial state: any first row is possible. */
    for (int c = 0; c < 5; ++c)
        for (int s = 0; s < 16; ++s)
            dp[c][s] = 1;

    /* Perform n steps (one step per 2×2 subblock). */
    for (idx_t step = 0; step < n; ++step) {
        for (int c = 0; c < 5; ++c) {
            for (int t = 0; t < 16; ++t)
                newdp[c][t] = 0;

            for (int s = 0; s < 16; ++s) {
                seq_t val = dp[c][s];
                if (val == 0) continue;
                for (int t = 0; t < 16; ++t) {
                    if (pre.mat[c][s][t])
                        newdp[c][t] += val;
                }
            }
            for (int t = 0; t < 16; ++t)
                dp[c][t] = newdp[c][t];
        }
    }

    /* Sum over all counts and all ending rows. */
    seq_t total = 0;
    for (int c = 0; c < 5; ++c)
        for (int s = 0; s < 16; ++s)
            total += dp[c][s];

    return total;
}
