/*
 * A000614  Number of complemented types of Boolean functions of n variables
 *          under action of AG(n,2).
 * https://oeis.org/A000614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (1/(2*|AGL(n,2)|)) * Sum_{g in AGL(n,2)} (1 + [all cycles
 *            of g even]) * 2^c(g), where c(g) = number of cycles of g on
 *            GF(2)^n. (Burnside's lemma for the affine group plus
 *            complementation.) The sum runs over conjugacy classes of GL(n,2)
 *            in rational canonical form. For each class, the transl...
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   z614_S, z614_cent, z614_divs, z614_gcd, z614_muls, z614_next,
 *            z614_ord, z614_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/z614_S.h"
#include "../utils/z614_cent.h"
#include "../utils/z614_divs.h"
#include "../utils/z614_gcd.h"
#include "../utils/z614_muls.h"
#include "../utils/z614_next.h"
#include "../utils/z614_ord.h"
#include "../utils/z614_phi.h"

typedef struct { uint64_t p, c; } z614_pc;

#define Z614_MAXD 36
#define Z614_DCAP 4096

static int Zn, Zns, Zcap;
static int *Zsd;
static uint64_t *Zse;
static size_t ZL, ZTW;
static uint32_t *ZT, *ZYs, *ZY0, *ZZ;
static z614_pc Zdist[Z614_MAXD][Z614_DCAP];
static z614_pc Ztmp[Z614_DCAP], Zcd[80], Zud[80];
static int Zdl[Z614_MAXD];
static int Zpl[Z614_MAXD][72];

/* ZT += Z << E */
static void z614_addsh(const uint32_t *Z, size_t zl, uint64_t E) {
    size_t word = (size_t)(E / 32), i, k;
    int bit = (int)(E % 32);
    uint64_t hi = 0, carry = 0;
    for (i = 0; i <= zl; i++) {
        uint64_t s = (i < zl) ? ((uint64_t)Z[i] << bit) : 0;
        uint64_t w = (s & 0xffffffffULL) | hi;
        uint64_t t;
        hi = s >> 32;
        t = (uint64_t)ZT[word + i] + w + carry;
        ZT[word + i] = (uint32_t)t;
        carry = t >> 32;
    }
    k = word + zl + 1;
    while (carry && k < ZTW) {
        uint64_t t = (uint64_t)ZT[k] + carry;
        ZT[k] = (uint32_t)t;
        carry = t >> 32;
        k++;
    }
}

/* period distribution of points for component f != x+1 (degree d, order e) */
static int z614_cdist(int d, uint64_t e, const int *lam, int r, z614_pc *o) {
    int k = 0, a, mx = r ? lam[0] : 0, prev = 0;
    o[k].p = 1; o[k].c = 1; k++;
    for (a = 0;; a++) {
        int t = 1 << a;
        int s = z614_S(lam, r, t);
        o[k].p = e << a;
        o[k].c = (1ULL << (d * s)) - (1ULL << (d * prev));
        k++;
        prev = s;
        if (t >= mx) break;
    }
    return k;
}

/* period distribution for unipotent part with affine translation parameter L */
static int z614_udist(const int *lam, int r, int L, z614_pc *o) {
    int k = 0, a, mx = r ? lam[0] : 0;
    uint64_t pf = 0;
    for (a = 0;; a++) {
        int t = 1 << a;
        uint64_t f = (t > L) ? (1ULL << z614_S(lam, r, t)) : 0;
        if (f > pf) { o[k].p = (uint64_t)t; o[k].c = f - pf; k++; }
        pf = f;
        if (t >= mx && t > L) break;
    }
    return k;
}

static int z614_comb(const z614_pc *x, int nx, const z614_pc *y, int ny, z614_pc *o) {
    int k = 0, i, j, t;
    for (i = 0; i < nx; i++)
        for (j = 0; j < ny; j++) {
            uint64_t g = z614_gcd(x[i].p, y[j].p);
            uint64_t p = x[i].p / g * y[j].p;
            uint64_t c = x[i].c * y[j].c;
            for (t = 0; t < k; t++) if (o[t].p == p) break;
            if (t < k) o[t].c += c;
            else if (k < Z614_DCAP) { o[k].p = p; o[k].c = c; k++; }
        }
    return k;
}

static void z614_proc(int D, int m, int e2acc) {
    int lam[72], r, i, j;
    lam[0] = m;
    r = m > 0 ? 1 : 0;
    do {
        int e2, base;
        memcpy(ZY0, ZYs + (size_t)D * ZL, ZL * sizeof(uint32_t));
        e2 = e2acc + z614_cent(1, lam, r, ZY0, ZL);
        base = Zn * (Zn - 1) / 2 - e2 + (Zn - m);
        for (i = -1; i < r; i++) {
            int L, logN = 0, nu, nt;
            uint64_t mult = 1, cyc = 0;
            if (i > 0 && lam[i] == lam[i - 1]) continue;
            if (i < 0) { L = 0; logN = m - r; }
            else {
                int cl = 0;
                L = lam[i];
                for (j = 0; j < r; j++) {
                    if (lam[j] > L) logN += lam[j] - 1;
                    else if (lam[j] < L) logN += lam[j];
                    else { logN += L - 1; cl++; }
                }
                mult = (1ULL << cl) - 1;
            }
            nu = z614_udist(lam, r, L, Zud);
            nt = z614_comb(Zdist[D], Zdl[D], Zud, nu, Ztmp);
            for (j = 0; j < nt; j++) cyc += Ztmp[j].c / Ztmp[j].p;
            memcpy(ZZ, ZY0, ZL * sizeof(uint32_t));
            ZZ[ZL] = 0;
            z614_muls(ZZ, ZL + 1, mult);
            z614_addsh(ZZ, ZL + 1, (uint64_t)base + (uint64_t)logN + cyc + (L > 0 ? 1 : 0));
        }
    } while (z614_next(lam, &r));
}

static void z614_node(int D, int si, int rem, int e2acc) {
    int j, k;
    z614_proc(D, rem, e2acc);
    for (j = si; j < Zns && Zsd[j] <= rem; j++) {
        int d = Zsd[j];
        for (k = 1; d * k <= rem; k++) {
            int *lam = Zpl[D];
            int r = 1;
            lam[0] = k;
            do {
                uint32_t *Yn = ZYs + (size_t)(D + 1) * ZL;
                int e2, nc;
                memcpy(Yn, ZYs + (size_t)D * ZL, ZL * sizeof(uint32_t));
                e2 = z614_cent(d, lam, r, Yn, ZL);
                nc = z614_cdist(d, Zse[j], lam, r, Zcd);
                Zdl[D + 1] = z614_comb(Zdist[D], Zdl[D], Zcd, nc, Zdist[D + 1]);
                z614_node(D + 1, j + 1, rem - d * k, e2acc + e2);
            } while (z614_next(lam, &r));
        }
    }
}

static void z614_push(int d, uint64_t e) {
    if (Zns == Zcap) {
        Zcap = Zcap ? 2 * Zcap : 64;
        Zsd = (int *)realloc(Zsd, (size_t)Zcap * sizeof(int));
        Zse = (uint64_t *)realloc(Zse, (size_t)Zcap * sizeof(uint64_t));
    }
    Zsd[Zns] = d;
    Zse[Zns] = e;
    Zns++;
}

seq_t a(idx_t n) {
    int N = (int)n, d, i, sh;
    long long j, maxbyte;
    seq_t res = 0;

    Zn = N;
    Zns = 0; Zcap = 0; Zsd = NULL; Zse = NULL;
    for (d = 2; d <= N; d++) {
        uint64_t M = (1ULL << d) - 1, e1;
        for (e1 = 1; e1 * e1 <= M; e1++) {
            if (M % e1 == 0) {
                uint64_t ee[2];
                int q, nq;
                ee[0] = e1; ee[1] = M / e1;
                nq = (ee[0] == ee[1]) ? 1 : 2;
                for (q = 0; q < nq; q++) {
                    uint64_t e = ee[q], cnt;
                    if (e < 3) continue;
                    if (z614_ord(e) != d) continue;
                    cnt = z614_phi(e) / (uint64_t)d;
                    while (cnt--) z614_push(d, e);
                }
            }
        }
    }

    ZL = (size_t)(N * (N + 1) / 2) / 32 + 2;
    ZYs = (uint32_t *)calloc((size_t)(Z614_MAXD + 1) * ZL, sizeof(uint32_t));
    ZY0 = (uint32_t *)calloc(ZL, sizeof(uint32_t));
    ZZ = (uint32_t *)calloc(ZL + 1, sizeof(uint32_t));
    ZYs[0] = 1;
    for (i = 1; i <= N; i++) z614_muls(ZYs, ZL, (1ULL << i) - 1);

    ZTW = ((size_t)N * N + 4 * (size_t)N + 8 + ((size_t)1 << N) + 32 * (ZL + 1) + 128) / 32 + 2;
    ZT = (uint32_t *)calloc(ZTW, sizeof(uint32_t));

    Zdist[0][0].p = 1; Zdist[0][0].c = 1; Zdl[0] = 1;
    z614_node(0, 0, N, 0);

    for (i = 1; i <= N; i++) {
        uint64_t f = (1ULL << i) - 1;
        if (f > 1) z614_divs(ZT, ZTW, f);
    }
    sh = N * (N - 1) / 2 + N + 1;

    maxbyte = (long long)ZTW * 4;
    for (j = maxbyte; j >= 0; j--) {
        uint64_t pos = (uint64_t)sh + 8 * (uint64_t)j;
        size_t w = (size_t)(pos / 32);
        int b = (int)(pos % 32);
        uint32_t v;
        if (w >= ZTW) continue;
        v = ZT[w] >> b;
        if (b > 24 && w + 1 < ZTW) v |= ZT[w + 1] << (32 - b);
        v &= 0xff;
        res = (seq_t)(res * 256);
        res += (idx_t)v;
    }

    free(ZT); free(ZYs); free(ZY0); free(ZZ); free(Zsd); free(Zse);
    ZT = NULL; ZYs = NULL; ZY0 = NULL; ZZ = NULL; Zsd = NULL; Zse = NULL;
    return res;
}
