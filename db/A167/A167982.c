/*
 * A167982  Number of n-cycles on the graph of the regular 16-cell, 3 <= n <=
 *          8.
 * https://oeis.org/A167982
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  fini, full, nonn, changed
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computation: brute‑force enumeration of all permutations of the 8 vertices of the
 * 16‑cell graph.  For each length 3 ≤ n ≤ 8 the number of directed n‑cycles
 * is counted via prefixes of the permutations; the final value is
 * (prefix_count[n]) / ((8‑n)! · 2n).  All other indices return 0.
 */

#include <stdbool.h>

/* adjacency matrix: 1 if vertices are adjacent in the 16‑cell graph */
static const bool adj[8][8] = {
    {0,0,1,1,1,1,1,1},
    {0,0,1,1,1,1,1,1},
    {1,1,0,0,1,1,1,1},
    {1,1,0,0,1,1,1,1},
    {1,1,1,1,0,0,1,1},
    {1,1,1,1,0,0,1,1},
    {1,1,1,1,1,1,0,0},
    {1,1,1,1,1,1,0,0}
};

static seq_t cycles[9] = {0};
static bool initialized = false;

/* depth‑first generation of all 8! permutations of 0…7 */
static void dfs_perm(int depth,
                     unsigned char *perm,
                     unsigned char *used,
                     seq_t *prefix)
{
    if (depth == 8) {
        /* evaluate all required lengths on the current permutation */
        for (int n = 3; n <= 8; ++n) {
            bool ok = true;
            for (int i = 0; i < n; ++i) {
                int v = perm[i];
                int w = perm[(i + 1) % n];
                if (!adj[v][w]) { ok = false; break; }
            }
            if (ok) ++prefix[n];
        }
        return;
    }
    for (int v = 0; v < 8; ++v) {
        if (!used[v]) {
            used[v] = 1;
            perm[depth] = (unsigned char)v;
            dfs_perm(depth + 1, perm, used, prefix);
            used[v] = 0;
        }
    }
}

static void compute_sequence(void)
{
    seq_t prefix[9] = {0};
    unsigned char perm[8];
    unsigned char used[8] = {0};

    dfs_perm(0, perm, used, prefix);

    /* factorial table for 0! … 8! */
    seq_t fact[9];
    fact[0] = 1;
    for (int i = 1; i <= 8; ++i) fact[i] = fact[i - 1] * (seq_t)i;

    for (int n = 3; n <= 8; ++n) {
        seq_t val = prefix[n];
        val = val / fact[8 - n];      /* divide by (8‑n)!          */
        val = val / (2 * n);          /* divide by 2n (undirected) */
        cycles[n] = val;
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n < 3 || n > 8) return 0;
    if (!initialized) compute_sequence();
    return cycles[n];
}
