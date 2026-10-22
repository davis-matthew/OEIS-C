/*
 * A003744  Number of Hamiltonian paths in O_5 X P_n.
 * https://oeis.org/A003744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Hamiltonian paths in (K_5 minus an edge) X P_n,
 *            computed by a layer-by-layer transfer matrix (states = how the
 *            open path pieces connect through the rungs, reduced by the
 *            graph's symmetries); terms past the first d use the linear
 *            recurrence a(n) = -Sum_{k=1..d} c_k a(n-k), where x...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   alloc_zero, find_, grow_zero, release, u128_to_seq, unite_
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/u128_to_seq.h"
#include "../utils/find_.h"
#include "../utils/unite_.h"

#define NV 5
#define NE 9
#define OUTM 5
#define NCODE 16807
#define MAXO 128

typedef unsigned __int128 u128_t;

static const int EDGES[NE][2] = {{0,2},{0,3},{0,4},{1,2},{1,3},{1,4},{2,3},{2,4},{3,4}};
static const int P3[6][3] = {{2,3,4},{2,4,3},{3,2,4},{3,4,2},{4,2,3},{4,3,2}};

static int perms[12][NV];
static int nperm;
static int code_to_orb[NCODE];
static int orb_code[MAXO];
static int norb;
static long long L[MAXO][MAXO];
static long long fin[MAXO];
static u128_t cp[MAXO + 1];

static int encode_(const int *p) {
    int c = 0, i;
    for (i = NV - 1; i >= 0; i--) c = c * 7 + (p[i] + 1);
    return c;
}

static void decode_(int c, int *p) {
    int i;
    for (i = 0; i < NV; i++) { p[i] = c % 7 - 1; c /= 7; }
}

static int canon_(const int *p) {
    int best = -1, g, i, c, q[NV];
    for (g = 0; g < nperm; g++) {
        for (i = 0; i < NV; i++) {
            int v = p[i];
            q[perms[g][i]] = (v >= 0 && v < OUTM) ? perms[g][v] : v;
        }
        c = encode_(q);
        if (best < 0 || c < best) best = c;
    }
    return best;
}

/* 0 = invalid, 1 = valid continuing (q filled), 2 = completes a Hamiltonian path */
static int step_(const int *p, int F, int T, int *q) {
    int deg[NV], par[NV], nup[NV], upa[NV], upb[NV];
    int i, e, r, ncomp = 0, outs = 0;
    for (i = 0; i < NV; i++) {
        deg[i] = (p[i] >= 0) + ((T >> i) & 1);
        par[i] = i; nup[i] = 0; upa[i] = upb[i] = -1;
    }
    for (e = 0; e < NE; e++)
        if ((F >> e) & 1) { deg[EDGES[e][0]]++; deg[EDGES[e][1]]++; }
    for (i = 0; i < NV; i++) if (deg[i] < 1 || deg[i] > 2) return 0;
    for (i = 0; i < NV; i++)
        if (p[i] > i && p[i] < OUTM) unite_(par, i, p[i]);
    for (e = 0; e < NE; e++)
        if ((F >> e) & 1)
            if (!unite_(par, EDGES[e][0], EDGES[e][1])) return 0;
    for (i = 0; i < NV; i++) {
        r = find_(par, i);
        if (r == i) ncomp++;
        if ((T >> i) & 1) {
            if (nup[r] == 0) upa[r] = i; else upb[r] = i;
            nup[r]++;
        }
    }
    if (T == 0) return ncomp == 1 ? 2 : 0;
    for (i = 0; i < NV; i++) q[i] = -1;
    for (i = 0; i < NV; i++) {
        if (find_(par, i) != i) continue;
        if (nup[i] == 0) return 0;
        if (nup[i] == 1) { q[upa[i]] = OUTM; outs++; }
        else { q[upa[i]] = upb[i]; q[upb[i]] = upa[i]; }
    }
    return outs <= 2 ? 1 : 0;
}

static void build_(void) {
    int p[NV], q[NV], o, F, T, r, c, i, x, y;
    nperm = 0;
    for (x = 0; x < 2; x++)
        for (y = 0; y < 6; y++) {
            perms[nperm][0] = x; perms[nperm][1] = 1 - x;
            perms[nperm][2] = P3[y][0]; perms[nperm][3] = P3[y][1]; perms[nperm][4] = P3[y][2];
            nperm++;
        }
    for (c = 0; c < NCODE; c++) code_to_orb[c] = -1;
    for (i = 0; i < NV; i++) p[i] = -1;
    c = canon_(p);
    code_to_orb[c] = 0; orb_code[0] = c; norb = 1;
    for (o = 0; o < norb; o++) {
        decode_(orb_code[o], p);
        for (F = 0; F < (1 << NE); F++)
            for (T = 0; T < (1 << NV); T++) {
                r = step_(p, F, T, q);
                if (r == 2) fin[o]++;
                else if (r == 1) {
                    c = canon_(q);
                    if (code_to_orb[c] < 0) {
                        code_to_orb[c] = norb; orb_code[norb] = c; norb++;
                    }
                    L[o][code_to_orb[c]]++;
                }
            }
    }
}

/* Samuelson-Berkowitz (division-free), arithmetic mod 2^128 */
static void charpoly_(void) {
    int n = norb, k, i, j, l, m;
    u128_t p[MAXO + 2], np[MAXO + 2], t[MAXO + 2], v[MAXO], w[MAXO];
    p[0] = 1;
    p[1] = (u128_t)0 - (u128_t)L[n - 1][n - 1];
    for (k = n - 2; k >= 0; k--) {
        m = n - 1 - k;
        t[0] = 1;
        t[1] = (u128_t)0 - (u128_t)L[k][k];
        for (i = 0; i < m; i++) v[i] = (u128_t)L[k + 1 + i][k];
        for (j = 0; j < m; j++) {
            u128_t s = 0;
            for (i = 0; i < m; i++) s += (u128_t)L[k][k + 1 + i] * v[i];
            t[j + 2] = (u128_t)0 - s;
            for (i = 0; i < m; i++) {
                u128_t s2 = 0;
                for (l = 0; l < m; l++) s2 += (u128_t)L[k + 1 + i][k + 1 + l] * v[l];
                w[i] = s2;
            }
            for (i = 0; i < m; i++) v[i] = w[i];
        }
        for (i = 0; i <= m + 1; i++) {
            u128_t s = 0;
            for (j = 0; j <= m && j <= i; j++) s += t[i - j] * p[j];
            np[i] = s;
        }
        for (i = 0; i <= m + 1; i++) p[i] = np[i];
    }
    for (i = 0; i <= n; i++) cp[i] = p[i];
}

static seq_t *A_ = 0;
static idx_t alen = 0, acap = 0;
static int ready = 0;
static int d_, nP = 0, nN = 0;
static int idxP[MAXO + 1], idxN[MAXO + 1];
static seq_t *cP, *cN;

static void setup_(void) {
    int k, s, t;
    idx_t n;
    seq_t *vec, *nv, *tmp;
    build_();
    charpoly_();
    d_ = norb;
    cP = alloc_zero((idx_t)d_ + 1);
    cN = alloc_zero((idx_t)d_ + 1);
    for (k = 1; k <= d_; k++) {
        __int128 v = -(__int128)cp[k];
        if (v > 0) { cP[k] = u128_to_seq((u128_t)v); idxP[nP++] = k; }
        else if (v < 0) { cN[k] = u128_to_seq((u128_t)(-v)); idxN[nN++] = k; }
    }
    acap = (idx_t)d_ + 16;
    A_ = alloc_zero(acap + 1);
    vec = alloc_zero((idx_t)d_);
    nv = alloc_zero((idx_t)d_);
    vec[0] = 1;
    for (n = 1; n <= (idx_t)d_; n++) {
        seq_t sum = 0;
        for (s = 0; s < d_; s++)
            if (fin[s]) sum += vec[s] * (idx_t)fin[s];
        A_[n] = sum;
        for (t = 0; t < d_; t++) nv[t] = 0;
        for (s = 0; s < d_; s++)
            for (t = 0; t < d_; t++)
                if (L[s][t]) nv[t] += vec[s] * (idx_t)L[s][t];
        tmp = vec; vec = nv; nv = tmp;
    }
    alen = (idx_t)d_;
    release(vec, (idx_t)d_);
    release(nv, (idx_t)d_);
}

seq_t a(idx_t n) {
    idx_t m;
    int j;
    if (!ready) { setup_(); ready = 1; }
    if (n > alen) {
        if (n > acap) {
            idx_t nc = acap * 2;
            if (nc < n) nc = n;
            A_ = grow_zero(A_, acap + 1, nc + 1);
            acap = nc;
        }
        for (m = alen + 1; m <= n; m++) {
            seq_t s = 0, t = 0;
            for (j = 0; j < nP; j++) s += cP[idxP[j]] * A_[m - (idx_t)idxP[j]];
            for (j = 0; j < nN; j++) t += cN[idxN[j]] * A_[m - (idx_t)idxN[j]];
            A_[m] = s - t;
        }
        alen = n;
    }
    return A_[n];
}
