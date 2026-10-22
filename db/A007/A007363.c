/*
 * A007363  Maximal self-dual antichains on n points.
 * https://oeis.org/A007363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1) = 0; for n > 1, a(n) is the number of maximal intersecting
 *            antichains of nonempty subsets of {1..n-1}. These are the
 *            maximal cliques of the graph on nonempty subsets where two sets
 *            are joined when they intersect and neither contains the other.
 *            The code counts them with a Bron-Kerbosch search...
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define A7363_MAXW 2
#define A7363_MAXM 7

static int a7363_W;
static uint64_t a7363_adj[1 << A7363_MAXM][A7363_MAXW];

static uint64_t a7363_bk(const uint64_t *P0, const uint64_t *X0) {
    uint64_t P[A7363_MAXW], X[A7363_MAXW], U[A7363_MAXW], cand[A7363_MAXW];
    int W = a7363_W, w, pc = 0, best = -1, bestc = -1;
    bool pempty = true, xempty = true;
    uint64_t cnt = 0;
    for (w = 0; w < W; w++) {
        P[w] = P0[w]; X[w] = X0[w];
        if (P[w]) pempty = false;
        if (X[w]) xempty = false;
        pc += __builtin_popcountll(P[w]);
        U[w] = P[w] | X[w];
    }
    if (pempty) return xempty ? 1 : 0;
    for (w = 0; w < W; w++) {
        uint64_t b = U[w];
        while (b) {
            int u = w * 64 + __builtin_ctzll(b);
            int c = 0, k;
            b &= b - 1;
            for (k = 0; k < W; k++) c += __builtin_popcountll(P[k] & a7363_adj[u][k]);
            if (c > bestc) {
                bestc = c; best = u;
                if (c == pc) {
                    if ((X[u >> 6] >> (u & 63)) & 1) return 0;
                    goto chosen;
                }
            }
        }
    }
chosen:
    for (w = 0; w < W; w++) cand[w] = P[w] & ~a7363_adj[best][w];
    for (w = 0; w < W; w++) {
        while (cand[w]) {
            int v = w * 64 + __builtin_ctzll(cand[w]);
            uint64_t NP[A7363_MAXW], NX[A7363_MAXW];
            int k;
            cand[w] &= cand[w] - 1;
            for (k = 0; k < W; k++) {
                NP[k] = P[k] & a7363_adj[v][k];
                NX[k] = X[k] & a7363_adj[v][k];
            }
            cnt += a7363_bk(NP, NX);
            P[v >> 6] &= ~((uint64_t)1 << (v & 63));
            X[v >> 6] |= (uint64_t)1 << (v & 63);
        }
    }
    return cnt;
}

static uint64_t a7363_count(int m) {
    int N = 1 << m, s, t, w;
    uint64_t P[A7363_MAXW], X[A7363_MAXW];
    uint64_t full = (uint64_t)(N - 1);
    uint64_t base = 0;
    if (m == 1) return 1;
    a7363_W = (N + 63) / 64;
    for (s = 0; s < N; s++)
        for (w = 0; w < A7363_MAXW; w++) a7363_adj[s][w] = 0;
    for (w = 0; w < A7363_MAXW; w++) { P[w] = 0; X[w] = 0; }
    for (s = 1; s < N; s++) {
        uint64_t us = (uint64_t)s;
        if (__builtin_popcountll(us) == 1 || us == full) { base++; continue; }
        P[s >> 6] |= (uint64_t)1 << (s & 63);
        for (t = 1; t < N; t++) {
            uint64_t ut = (uint64_t)t, it = us & ut;
            if (s != t && it != 0 && it != us && it != ut)
                a7363_adj[s][t >> 6] |= (uint64_t)1 << (t & 63);
        }
    }
    {
        bool any = false;
        for (w = 0; w < A7363_MAXW; w++) if (P[w]) any = true;
        if (!any) return base;
    }
    return base + a7363_bk(P, X);
}

seq_t a(idx_t n) {
    static uint64_t memo[A7363_MAXM + 2];
    static bool have[A7363_MAXM + 2];
    int m;
    if (n <= 1) return (seq_t)0;
    if (n - 1 > A7363_MAXM) return (seq_t)0;
    m = (int)(n - 1);
    if (!have[m]) {
        memo[m] = a7363_count(m);
        have[m] = true;
    }
    return (seq_t)memo[m];
}
