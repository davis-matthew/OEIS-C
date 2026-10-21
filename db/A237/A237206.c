/*
 * A237206  Number of (n+1)X(3+1) 0..3 arrays with the upper median plus the
 *          lower median minus the minimum of every 2X2 subblock equal.
 * https://oeis.org/A237206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*--------------------------------------------------------------------
   OEIS A237206
   Number of (n+1)×4 arrays with entries 0..3 such that
   upper median + lower median – minimum of every 2×2 subblock is
   the same constant k (for some k between 0 and 6).
   The value a(n) is the total number of such arrays for all k.
--------------------------------------------------------------------
   This implementation precomputes, for every k, the directed graph
   on the 256 possible rows.  An edge (r1 → r2) exists exactly when
   the three overlapping 2×2 blocks formed by stacking r1 above r2
   all yield the same value k.  Then a(n) is obtained by counting
   all paths of length n in every graph and summing the counts.
   The counting is performed with a simple dynamic‑programming
   recursion that only uses addition, therefore it works for
   arbitrary‑precision seq_t as well as for built‑in integer types.
--------------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>

/* Row values ----------------------------------------------------*/
static uint8_t row_vals[256][4];

/* adjacency: adj_to[K][from][d] is a destination for the d‑th edge
   from "from" in graph K (0 ≤ K ≤ 6).  adj_deg[K][from] holds the
   number of outgoing edges.  All indices fit in 256, so 8 bits are
   sufficient for degrees.                                                 */
static uint16_t adj_to[7][256][256];
static uint8_t adj_deg[7][256];
static bool  precomp_done = false;

/* Compute the constant k for a pair of rows ---------*/
static inline bool
compute_k(uint8_t a, uint8_t b, uint8_t *k_out)
{
    int va[4] = { row_vals[a][0], row_vals[a][1],
                  row_vals[a][2], row_vals[a][3] };
    int vb[4] = { row_vals[b][0], row_vals[b][1],
                  row_vals[b][2], row_vals[b][3] };

    int k = -1;
    for (int j = 0; j < 3; ++j) {
        int v[4] = { va[j], va[j + 1], vb[j], vb[j + 1] };

        /* simple sorting network for 4 values */
        int t0 = v[0], t1 = v[1], t2 = v[2], t3 = v[3];
        if (t1 < t0) { int t = t1; t1 = t0; t0 = t; }
        if (t2 < t0) { int t = t2; t2 = t0; t0 = t; }
        if (t2 < t1) { int t = t2; t2 = t1; t1 = t; }
        if (t3 < t0) { int t = t3; t3 = t0; t0 = t; }
        if (t3 < t1) { int t = t3; t3 = t1; t1 = t; }
        if (t3 < t2) { int t = t3; t3 = t2; t2 = t; }

        int kk = t2 + t1 - t0;   /* m2 + m1 – min */

        if (j == 0) k = kk;
        else if (k != kk) return false;
    }
    *k_out = (uint8_t)k;
    return true;
}

/* Pre–compute adjacency lists -----------------------------------*/
static void
precompute(void)
{
    if (precomp_done) return;
    precomp_done = true;

    /* build row_vals from 0..255 */
    for (int a = 0; a < 256; ++a) {
        row_vals[a][0] =  a         & 3;
        row_vals[a][1] = (a >> 2)   & 3;
        row_vals[a][2] = (a >> 4)   & 3;
        row_vals[a][3] = (a >> 6)   & 3;
    }

    /* enumerate all ordered row pairs */
    for (int a = 0; a < 256; ++a) {
        for (int b = 0; b < 256; ++b) {
            uint8_t k;
            if (compute_k((uint8_t)a, (uint8_t)b, &k)) {
                uint8_t d = adj_deg[k][a];
                adj_to[k][a][d] = (uint16_t)b;
                ++adj_deg[k][a];
            }
        }
    }
}

/* Main sequence function ----------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined only for n ≥ 1, but safe */

    precompute();

    seq_t total = 0;

    /* temporary arrays for DP */
    seq_t cur[256];
    seq_t nxt[256];

    for (int K = 0; K <= 6; ++K) {

        /* initial state: any first row possible */
        for (int i = 0; i < 256; ++i) cur[i] = 1;

        /* walk through n steps (number of 2×2 blocks) */
        for (idx_t step = 0; step < n; ++step) {
            /* zero nxt */
            for (int i = 0; i < 256; ++i) nxt[i] = 0;

            /* transition according to adjacency list of graph K */
            for (int from = 0; from < 256; ++from) {
                seq_t val = cur[from];
                if (val == 0) continue;               /* skip empty paths */
                uint8_t deg = adj_deg[K][from];
                for (uint8_t d = 0; d < deg; ++d) {
                    uint16_t to = adj_to[K][from][d];
                    nxt[to] += val;
                }
            }

            /* copy nxt to cur for next iteration */
            for (int i = 0; i < 256; ++i) cur[i] = nxt[i];
        }

        /* sum all ending rows for this K */
        seq_t sumK = 0;
        for (int i = 0; i < 256; ++i) sumK += cur[i];
        total += sumK;
    }

    return total;
}
