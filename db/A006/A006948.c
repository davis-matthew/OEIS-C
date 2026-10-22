/*
 * A006948  Number of zero-entropy permutations of length n.
 * https://oeis.org/A006948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   exhaustive search -- a(n) counts permutations p of 1..n whose
 *            connect-the-dots interval map has zero topological entropy, i.e.
 *            whose Markov graph on the intervals [i,i+1] (I_i -> I_j iff
 *            p([i,i+1]) covers I_j) has spectral radius <= 1: every strongly
 *            connected component holds at most one cycle (e...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   zpc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/zpc.h"

static int zN;
static uint64_t zadj[64];
static int zperm[64];
static uint64_t zcnt;

/* check the SCC containing vertex s (the only one that can have changed) */
static bool zok(int s)
{
    uint64_t r[64];
    int m = zN - 1, i, k, u;
    for (i = 0; i < m; i++) r[i] = zadj[i] | (1ULL << i);
    for (k = 0; k < m; k++)
        for (i = 0; i < m; i++)
            if ((r[i] >> k) & 1ULL) r[i] |= r[k];
    uint64_t scc = 0;
    for (u = 0; u < m; u++)
        if (((r[s] >> u) & 1ULL) && ((r[u] >> s) & 1ULL)) scc |= 1ULL << u;
    int e = 0;
    for (u = 0; u < m; u++)
        if ((scc >> u) & 1ULL) e += zpc(zadj[u] & scc);
    return e <= zpc(scc);
}

static void zrec(int k, uint64_t used)
{
    int v;
    if (k == zN) { zcnt++; return; }
    for (v = 0; v < zN; v++) {
        if ((used >> v) & 1ULL) continue;
        zperm[k] = v;
        if (k >= 1) {
            int lo = zperm[k - 1], hi = v;
            if (lo > hi) { int t = lo; lo = hi; hi = t; }
            zadj[k - 1] = ((1ULL << hi) - 1) ^ ((1ULL << lo) - 1);
            if (!zok(k - 1)) continue;
        }
        zrec(k + 1, used | (1ULL << v));
    }
    if (k >= 1) zadj[k - 1] = 0;
}

seq_t a(idx_t n)
{
    static seq_t memo[64];
    static bool done[64];
    int i;
    if (n < 1) return (seq_t)0;
    if (n > 63) return (seq_t)0;
    if (done[n]) return memo[n];
    zN = (int)n;
    zcnt = 0;
    for (i = 0; i < 64; i++) zadj[i] = 0;
    zrec(0, 0);
    memo[n] = (seq_t)zcnt;
    done[n] = true;
    return memo[n];
}
