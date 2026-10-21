/*
 * A237135  Number of (n+1)X(1+1) 0..3 arrays with the maximum plus the upper
 *          median of every 2X2 subblock equal.
 * https://oeis.org/A237135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of (n+1)×2 arrays with entries 0..3 such that for every
 * 2×2 subblock the value (maximum + upper median) is the same for all subblocks.
 * The implementation builds, once, the 7 adjacency matrices (for sums 0..6)
 * corresponding to those 2×2 subblocks, then performs a simple DP for each n.
 * For each sum the number of admissible arrays equals the sum of all entries of
 * the matrix raised to the n‑th power, where n is the number of 2×2 subblocks.
 * This computation is performed iteratively in O(7·256·n) time, which is
 * comfortably below the 2 s limit for n≈10 000. */

#include <stdint.h>
#include <stdbool.h>

static unsigned char adj[7][16][16];          /* 0/1 adjacency, per sum 0..6 */

static void init_adj(void)
{
    static bool inited = false;
    if (inited) return;

    for (int s = 0; s < 7; ++s) {
        for (int u = 0; u < 16; ++u) {
            int a1 = u >> 2;      /* high two bits of row u */
            int b1 = u & 3;        /* low  two bits of row u */
            for (int v = 0; v < 16; ++v) {
                int a2 = v >> 2;
                int b2 = v & 3;
                /* compute sum = max + upper median of a1,b1,a2,b2 */
                int vals[4] = {a1, b1, a2, b2};
                int max1 = -1, max2 = -1;
                for (int k = 0; k < 4; ++k) {
                    int x = vals[k];
                    if (x > max1) { max2 = max1; max1 = x; }
                    else if (x > max2) { max2 = x; }
                }
                int sum = max1 + max2;   /* max + upper median */
                adj[s][u][v] = (sum == s) ? 1 : 0;
            }
        }
    }
    inited = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset 1: undefined below 1 */

    init_adj();

    seq_t total = 0;

    for (int s = 0; s < 7; ++s) {
        seq_t cur[16];
        seq_t nxt[16];
        /* start with one array that has any first row (all 16 possibilities) */
        for (int i = 0; i < 16; ++i) cur[i] = (seq_t)1;

        /* perform n transitions (there are n subblocks) */
        for (idx_t step = 1; step <= n; ++step) {
            for (int j = 0; j < 16; ++j) nxt[j] = (seq_t)0;
            for (int i = 0; i < 16; ++i) {
                seq_t c = cur[i];
                if (c == 0) continue;
                for (int j = 0; j < 16; ++j) {
                    if (adj[s][i][j]) nxt[j] += c;
                }
            }
            /* copy nxt to cur for next iteration */
            for (int i = 0; i < 16; ++i) cur[i] = nxt[i];
        }

        /* sum all counts for this sum */
        for (int i = 0; i < 16; ++i) total += cur[i];
    }
    return total;
}
