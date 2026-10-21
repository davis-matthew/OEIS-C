/*
 * A237219  Number of (n+1)X(1+1) 0..3 arrays with the maximum plus the lower
 *          median minus the upper median of every 2X2 subblock equal.
 * https://oeis.org/A237219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   compute_E
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237219: number of (n+1)×2 arrays of digits 0‑3 such that
 * every 2×2 subblock has the same value of
 *   max + lower‑median – upper‑median.
 *
 * This implementation builds for each of the four possible values
 * the directed graph of allowable 2‑row transitions and counts
 * walks of length n (i.e., arrays of height n+1) by a simple
 * dynamic‑programming loop.  No run‑time allocation of arbitrary‑size
 * objects is required, so the same code compiles both as C and
 * as C++ with an arbitrary‑precision integer class for seq_t.
 *
 * Complexity:  O(4·16²·n) ≈ 10⁷ basic operations for n=10 000,
 * far below the 2 s limit.  No memoisation is used – the value
 * is recomputed on every call, which is safe for any call order.
 */

#include <stdint.h>   /* for uint16_t */
#include "../utils/compute_E.h"

/* --------------------------------------------------------------------- */
/* value of a 2×2 block, coded by two rows of length 2 (values 0‑3)      */

/* --------------------------------------------------------------------- */
/* adjacency masks for the 4 graphs                                  */
/* --------------------------------------------------------------------- */
static uint16_t adjMask[4][16];
static int adj_initialized = 0;

static void init_adjacency(void)
{
    for (int c = 0; c < 4; ++c)
        for (int j = 0; j < 16; ++j)
            adjMask[c][j] = 0;       /* clear all bits */

    for (int j = 0; j < 16; ++j) {
        for (int k = 0; k < 16; ++k) {
            unsigned char e = compute_E((uint8_t)j, (uint8_t)k);
            adjMask[e][j] |= (uint16_t)(1u << k);
        }
    }
    adj_initialized = 1;
}

/* --------------------------------------------------------------------- */
/* main sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise once */
    if (!adj_initialized)
        init_adjacency();

    if (n == 0)
        return 0;          /* offset starts at 1 */

    seq_t total = 0;       /* sum over the 4 value graphs */

    /* temporary DP arrays */
    seq_t dp[16], next[16];

    for (int c = 0; c < 4; ++c) {
        /* start: one walk of length 0 in every state */
        for (int i = 0; i < 16; ++i)
            dp[i] = 1;

        /* iterate over the n transitions */
        for (idx_t step = 1; step <= n; ++step) {
            /* clear next */
            for (int i = 0; i < 16; ++i)
                next[i] = 0;

            /* propagate transitions */
            for (int j = 0; j < 16; ++j) {
                seq_t val = dp[j];
                if (val == 0)
                    continue;
                uint16_t mask = adjMask[c][j];
                for (int i = 0; i < 16; ++i)
                    if (mask & (1u << i))
                        next[i] += val;
            }

            /* swap dp and next */
            for (int i = 0; i < 16; ++i)
                dp[i] = next[i];
        }

        /* sum all walks that end in any state */
        for (int i = 0; i < 16; ++i)
            total += dp[i];
    }

    return total;
}
