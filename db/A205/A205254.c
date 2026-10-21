/*
 * A205254  Number of (n+1)X8 0..1 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock the same.
 * https://oeis.org/A205254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of (n+1)×8 0..1 arrays in which * every 2×2
 *            subblock has the same number of clockwise edge increases. * The
 *            sequence is computed by dynamic programming over the rows. * For
 *            each possible constant count (0..4) we pre‑compute all
 *            admissible * pairs of consecutive rows and then co...
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* adjacency lists for each possible constant count (0..4) */
static uint8_t adj_from[5][65536];
static uint8_t adj_to[5][65536];
static uint32_t adj_cnt[5] = {0};
static bool adj_built = false;

/* Precompute all admissible row pairs.  Each row is an 8‑bit pattern. */
static void build_adjacency(void)
{
    for (int a = 0; a < 256; ++a) {
        for (int b = 0; b < 256; ++b) {
            int count = -1;
            bool ok = true;
            for (int j = 0; j < 7; ++j) {
                int a_j   = (a >> j)     & 1;
                int a_j1  = (a >> (j+1)) & 1;
                int b_j   = (b >> j)     & 1;
                int b_j1  = (b >> (j+1)) & 1;
                int inc   = (a_j  < a_j1)
                         + (a_j1 <  b_j1)
                         + (b_j1 <  b_j )
                         + (b_j  <  a_j );
                if (j == 0)
                    count = inc;
                else if (inc != count) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                uint32_t idx = adj_cnt[count];
                adj_from[count][idx] = (uint8_t)a;
                adj_to[count][idx]   = (uint8_t)b;
                adj_cnt[count]++;
            }
        }
    }
    adj_built = true;
}

seq_t a(idx_t n)
{
    if (!adj_built)
        build_adjacency();

    seq_t result = 0;

    for (int C = 0; C < 5; ++C) {
        seq_t dp[256];
        seq_t newdp[256];

        /* initialise: one way to start with any row */
        for (int i = 0; i < 256; ++i)
            dp[i] = 1;

        /* perform n transitions (n = number of 2×2 blocks a(n) has) */
        for (idx_t step = 0; step < n; ++step) {
            for (int i = 0; i < 256; ++i)
                newdp[i] = 0;

            uint32_t ec = adj_cnt[C];
            for (uint32_t k = 0; k < ec; ++k) {
                uint8_t src = adj_from[C][k];
                uint8_t dst = adj_to[C][k];
                newdp[dst] += dp[src];
            }

            /* copy newdp into dp for next iteration */
            for (int i = 0; i < 256; ++i)
                dp[i] = newdp[i];
        }

        /* sum over all ending rows */
        for (int i = 0; i < 256; ++i)
            result += dp[i];
    }

    return result;
}
