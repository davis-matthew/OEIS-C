/*
 * A205174  Number of (n+1)X2 0..3 arrays with the permanents of 2X2 subblocks
 *          nondecreasing rightwards and downwards.
 * https://oeis.org/A205174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over row pairs; no closed formula. */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* If the index is below the defined offset, return 0. */
    if (n == 0) return 0;

    /* One-time initialization of transition tables. */
    static int initialized = 0;
    static unsigned char aVal[16];
    static unsigned char bVal[16];
    static unsigned char perm[16][16];
    static unsigned char nexts[256][16];
    static unsigned char deg[256];

    if (!initialized) {
        /* Map index 0..15 to pairs (a,b) where a,b in {0,1,2,3}. */
        for (int i = 0; i < 16; ++i) {
            aVal[i] = (unsigned char)(i / 4);
            bVal[i] = (unsigned char)(i % 4);
        }

        /* Permanent of a 2×2 block from rows i and j. */
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 16; ++j) {
                unsigned int p = (unsigned int)aVal[i] * bVal[j]
                               + (unsigned int)bVal[i] * aVal[j];
                perm[i][j] = (unsigned char)p;    /* p <= 18 */
            }
        }

        /* Build adjacency lists: from state (prev,curr) to next states
           that satisfy perm(prev,curr) <= perm(curr,next). */
        for (int prev = 0; prev < 16; ++prev) {
            for (int curr = 0; curr < 16; ++curr) {
                int s = prev * 16 + curr;
                deg[s] = 0;
                unsigned char p01 = perm[prev][curr];
                for (int next = 0; next < 16; ++next) {
                    if (p01 <= perm[curr][next]) {
                        nexts[s][deg[s]++] = (unsigned char)next;
                    }
                }
            }
        }
        initialized = 1;
    }

    /* DP arrays: dp holds counts for current prefix ending with a pair (prev,curr). */
    seq_t dp[256];
    seq_t nxt[256];

    /* Initialize: for the first two rows, every pair (x0,x1) is possible. */
    for (int s = 0; s < 256; ++s) dp[s] = 1;

    /* Perform n-1 transitions: add each subsequent row. */
    for (idx_t step = 0; step < n - 1; ++step) {
        /* Zero the next array. */
        for (int s = 0; s < 256; ++s) nxt[s] = 0;

        /* Transition step. */
        for (int s = 0; s < 256; ++s) {
            seq_t ways = dp[s];
            if (ways == 0) continue;
            int curr = s % 16;
            int d = deg[s];
            for (int k = 0; k < d; ++k) {
                int next = nexts[s][k];
                int dest = curr * 16 + next;       /* new state (curr,next) */
                nxt[dest] += ways;
            }
        }

        /* Prepare for next iteration. */
        for (int s = 0; s < 256; ++s) dp[s] = nxt[s];
    }

    /* Sum over all possible ending states. */
    seq_t result = 0;
    for (int s = 0; s < 256; ++s) {
        result += dp[s];
    }

    return result;
}
