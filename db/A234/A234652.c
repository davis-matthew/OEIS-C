/*
 * A234652  Number of (n+1) X (2+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A234652
 * Formula: a(n) is the number of (n+1)×3 arrays with entries 0..5 such that
 * for every 2×2 subblock the absolute difference between the diagonal and
 * antidiagonal sums equals 4.  The value is computed by dynamic programming
 * on the graph of allowed row transitions.  Each row is a triple (x,y,z)
 * with 0≤x,y,z≤5 (216 possible rows).  Two rows a=(a0,a1,a2) and b=(b0,b1,b2)
 * are compatible if
 *   |(a0 + b1) – (a1 + b0)| = 4   and
 *   |(a1 + b2) – (a2 + b1)| = 4.
 * The graph’s adjacency is pre–computed once, and a(n) is obtained by
 * iterating n transitions starting from a uniform vector of 1’s. */

/* The build adjacency step uses at most 24 outgoing edges per node,
 * 216 nodes total, so the per‑step work is about 5k additions.
 * For n up to 10 000 this is well below the time limit. */

#include <stdint.h>
#include <stdbool.h>

/* --- adjacency data ---------------------------------------------- */
static bool  adj_init   = false;
static uint16_t  outdeg[216];
static uint16_t  outs[216][24];   /* max 24 transitions per source */

/* Build the adjacency graph once. */
static void build_adjacency(void)
{
    for (int a0 = 0; a0 < 6; ++a0)
        for (int a1 = 0; a1 < 6; ++a1)
            for (int a2 = 0; a2 < 6; ++a2) {
                int src = a0 * 36 + a1 * 6 + a2; /* unique row index */
                int deg = 0;
                for (int s1 = -1; s1 <= 1; s1 += 2)
                    for (int s2 = -1; s2 <= 1; s2 += 2) {
                        int d1 = s1 * 4 - (a0 - a1);
                        int d2 = s2 * 4 - (a1 - a2);
                        for (int b0 = 0; b0 < 6; ++b0) {
                            int b1 = b0 + d1;
                            if (b1 < 0 || b1 > 5) continue;
                            int b2 = b1 + d2;
                            if (b2 < 0 || b2 > 5) continue;
                            int dest = b0 * 36 + b1 * 6 + b2;
                            outs[src][deg++] = (uint16_t)dest;
                        }
                    }
                outdeg[src] = (uint16_t)deg;
            }
    adj_init = true;
}

/* --- sequence computation ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    if (!adj_init) build_adjacency();

    static seq_t cur[216];
    static seq_t nxt[216];

    /* start vector: all ones (arrays of length 1) */
    for (int i = 0; i < 216; ++i) {
        cur[i] = (seq_t)1;
        nxt[i] = (seq_t)0;
    }

    /* perform n transitions: after n steps we have n+1 rows */
    for (idx_t step = 0; step < n; ++step) {
        /* zero next vector */
        for (int i = 0; i < 216; ++i) nxt[i] = (seq_t)0;

        /* transition */
        for (int i = 0; i < 216; ++i) {
            seq_t val = cur[i];
            if (val == 0) continue;
            int deg = outdeg[i];
            for (int k = 0; k < deg; ++k) {
                int dst = outs[i][k];
                nxt[dst] += val;
            }
        }

        /* copy next back into cur for next iteration */
        for (int i = 0; i < 216; ++i) cur[i] = nxt[i];
    }

    /* sum all counts: total number of arrays */
    seq_t res = (seq_t)0;
    for (int i = 0; i < 216; ++i) res += cur[i];
    return res;
}
