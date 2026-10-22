/*
 * A007813  Number of planar Steinhaus graphs with n nodes.
 * https://oeis.org/A007813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of first rows s (2^(n-1) choices) whose Steinhaus
 *            graph (a[i][j] = a[i-1][j-1] XOR a[i-1][j]) is planar, counted
 *            by exhaustive search with a Demoucron-Malgrange-Pertuiset
 *            planarity test; a(n) = a(16) = 26 for n>15
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   low32, popc32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/low32.h"
#include "../utils/popc32.h"

static uint32_t G[16];

static uint32_t comp_of(uint32_t M, int s)
{
    uint32_t seen = 1u << s, fr = seen;
    while (fr) {
        uint32_t nf = 0, t = fr;
        while (t) { int v = low32(t); t &= t - 1; nf |= G[v] & M; }
        nf &= ~seen; seen |= nf; fr = nf;
    }
    return seen;
}

static bool dmp(uint32_t M)
{
    static int face[64][20];
    int flen[64], nf = 0;
    uint32_t fmask[64], emb[16], H = 0;
    int par[16], q[16], qh, qt, i, k;
    for (i = 0; i < 16; i++) emb[i] = 0;

    /* initial cycle */
    {
        int u = low32(M), w = low32(G[u] & M), x;
        for (i = 0; i < 16; i++) par[i] = -2;
        par[w] = -1; qh = qt = 0; q[qt++] = w;
        while (qh < qt) {
            uint32_t nb;
            x = q[qh++];
            nb = G[x] & M;
            if (x == w) nb &= ~(1u << u);
            while (nb) {
                int y = low32(nb); nb &= nb - 1;
                if (par[y] == -2) { par[y] = x; q[qt++] = y; }
            }
            if (par[u] != -2) break;
        }
        flen[0] = 0; fmask[0] = 0;
        x = u;
        while (x != -1) { face[0][flen[0]++] = x; fmask[0] |= 1u << x; x = par[x]; }
        for (i = 0; i < flen[0]; i++) {
            int p = face[0][i], r = face[0][(i + 1) % flen[0]];
            emb[p] |= 1u << r; emb[r] |= 1u << p;
        }
        H = fmask[0];
        flen[1] = flen[0]; fmask[1] = fmask[0];
        for (i = 0; i < flen[0]; i++) face[1][i] = face[0][i];
        nf = 2;
    }

    for (;;) {
        int bestCount = INT_MAX, bestFace = -1, bestLen = 0, bestPath[20];
        uint32_t t, rest;
        /* type a fragments: single edges between H vertices */
        t = H;
        while (t && bestCount > 1) {
            int u = low32(t); uint32_t vs;
            t &= t - 1;
            vs = G[u] & M & H & ~emb[u];
            while (vs) {
                int v = low32(vs), cnt = 0, ff = -1, f;
                uint32_t att;
                vs &= vs - 1;
                if (v < u) continue;
                att = (1u << u) | (1u << v);
                for (f = 0; f < nf; f++)
                    if ((fmask[f] & att) == att) { if (ff < 0) ff = f; cnt++; }
                if (cnt == 0) return false;
                if (cnt < bestCount) {
                    bestCount = cnt; bestFace = ff;
                    bestPath[0] = u; bestPath[1] = v; bestLen = 2;
                }
            }
        }
        /* type b fragments: components of G - H */
        rest = M & ~H;
        while (rest) {
            uint32_t K = comp_of(rest, low32(rest)), att = 0, kk = K;
            int cnt = 0, ff = -1, f;
            rest &= ~K;
            while (kk) { int x = low32(kk); kk &= kk - 1; att |= G[x] & H; }
            for (f = 0; f < nf; f++)
                if ((fmask[f] & att) == att) { if (ff < 0) ff = f; cnt++; }
            if (cnt == 0) return false;
            if (cnt < bestCount) {
                int a1 = low32(att), a2 = -1, end = -1, x;
                uint32_t st = G[a1] & K;
                bestCount = cnt; bestFace = ff;
                for (i = 0; i < 16; i++) par[i] = -2;
                qh = qt = 0;
                while (st) { int y = low32(st); st &= st - 1; par[y] = a1; q[qt++] = y; }
                while (qh < qt) {
                    uint32_t o, nb;
                    x = q[qh++];
                    o = G[x] & att & ~(1u << a1);
                    if (o) { a2 = low32(o); end = x; break; }
                    nb = G[x] & K;
                    while (nb) {
                        int y = low32(nb); nb &= nb - 1;
                        if (par[y] == -2) { par[y] = x; q[qt++] = y; }
                    }
                }
                if (a2 < 0) return false; /* cannot happen in a biconnected graph */
                bestLen = 0;
                bestPath[bestLen++] = a2;
                x = end;
                while (x != a1) { bestPath[bestLen++] = x; x = par[x]; }
                bestPath[bestLen++] = a1;
            }
        }
        if (bestFace < 0) return true;

        /* embed path into face */
        {
            int f = bestFace, L = flen[f], fi = -1, fj = -1;
            int p0 = bestPath[0], pl = bestPath[bestLen - 1];
            int n1[20], n2[20], l1 = 0, l2 = 0;
            uint32_t m1 = 0, m2 = 0;
            for (i = 0; i < L; i++) {
                if (face[f][i] == p0) fi = i;
                if (face[f][i] == pl) fj = i;
            }
            k = fi;
            for (;;) { n1[l1++] = face[f][k]; if (k == fj) break; k = (k + 1) % L; }
            for (i = bestLen - 2; i >= 1; i--) n1[l1++] = bestPath[i];
            k = fj;
            for (;;) { n2[l2++] = face[f][k]; if (k == fi) break; k = (k + 1) % L; }
            for (i = 1; i <= bestLen - 2; i++) n2[l2++] = bestPath[i];
            for (i = 0; i < l1; i++) { face[f][i] = n1[i]; m1 |= 1u << n1[i]; }
            flen[f] = l1; fmask[f] = m1;
            for (i = 0; i < l2; i++) { face[nf][i] = n2[i]; m2 |= 1u << n2[i]; }
            flen[nf] = l2; fmask[nf] = m2; nf++;
            for (i = 0; i + 1 < bestLen; i++) {
                int p = bestPath[i], r = bestPath[i + 1];
                emb[p] |= 1u << r; emb[r] |= 1u << p;
                H |= (1u << p) | (1u << r);
            }
        }
    }
}

static bool planar_mask(uint32_t M)
{
    bool changed = true;
    uint32_t t, c;
    int V, E;
    while (changed) {
        changed = false;
        t = M;
        while (t) {
            int v = low32(t); t &= t - 1;
            if (popc32(G[v] & M) <= 1) { M &= ~(1u << v); changed = true; }
        }
    }
    if (popc32(M) <= 4) return true;
    c = comp_of(M, low32(M));
    if (c != M) return planar_mask(c) && planar_mask(M & ~c);
    t = M;
    while (t) {
        int v = low32(t);
        uint32_t R = M & ~(1u << v);
        t &= t - 1;
        c = comp_of(R, low32(R));
        if (c != R) {
            uint32_t rest = R;
            while (rest) {
                c = comp_of(rest, low32(rest));
                if (!planar_mask(c | (1u << v))) return false;
                rest &= ~c;
            }
            return true;
        }
    }
    V = popc32(M); E = 0;
    t = M;
    while (t) { int v = low32(t); t &= t - 1; E += popc32(G[v] & M); }
    E /= 2;
    if (E > 3 * V - 6) return false;
    return dmp(M);
}

static long long count_planar(int n)
{
    long long cnt = 0;
    uint32_t s, total = 1u << (n - 1), full = (n == 32) ? 0xFFFFFFFFu : ((1u << n) - 1u);
    for (s = 0; s < total; s++) {
        uint32_t r = s << 1;
        int i, E = 0;
        for (i = 0; i < n; i++) G[i] = 0;
        for (i = 0; i < n; i++) {
            uint32_t above = full & ~((2u << i) - 1u);
            uint32_t b;
            if (i > 0) r = ((r << 1) ^ r) & above;
            else r &= above;
            b = r;
            while (b) {
                int j = low32(b); b &= b - 1;
                G[i] |= 1u << j; G[j] |= 1u << i; E++;
            }
        }
        if (n >= 3 && E > 3 * n - 6) continue;
        if (planar_mask(full)) cnt++;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    static long long memo[17];
    static bool done[17];
    int m = (n > 16) ? 16 : (int)n;
    if (!done[m]) { memo[m] = count_planar(m); done[m] = true; }
    return (seq_t)memo[m];
}
