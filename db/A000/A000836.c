/*
 * A000836  Number of switching networks (see Harrison reference for precise
 *          definition).
 * https://oeis.org/A000836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(6*|AG(n,2)|)) * Sum_{g in AG(n,2)} Sum_{s in S_3}
 *            2^(c_s(g)), summed by Burnside over affine conjugacy classes
 *            (unipotent part on F_2^(k+1) restricted to the hyperplane phi=1,
 *            times primary rational-canonical blocks), where c_s(g) is the
 *            number of cycles of (g,s) acting on the maps F_2...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   a836_cent, a836_clog, a836_deg, a836_g, a836_pmod, a836_pw2,
 *            a836_summin
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a836_cent.h"
#include "../utils/a836_clog.h"
#include "../utils/a836_deg.h"
#include "../utils/a836_g.h"
#include "../utils/a836_pmod.h"
#include "../utils/a836_pw2.h"
#include "../utils/a836_summin.h"

typedef struct { uint64_t *per; uint64_t *cnt; int n; } a836_dist;
typedef struct { int **p; int *len; int n, cap; } a836_plist;

static a836_dist a836_new(int cap) {
    a836_dist D;
    D.per = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(cap + 1));
    D.cnt = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(cap + 1));
    D.n = 0;
    return D;
}
static void a836_free(a836_dist *D) { free(D->per); free(D->cnt); }
static void a836_add(a836_dist *D, uint64_t per, uint64_t c) {
    int t;
    if (c == 0) return;
    for (t = 0; t < D->n; t++) if (D->per[t] == per) { D->cnt[t] += c; return; }
    D->per[D->n] = per; D->cnt[D->n] = c; D->n++;
}
static a836_dist a836_conv(const a836_dist *A, const a836_dist *B) {
    a836_dist R = a836_new(A->n * B->n);
    int i, j;
    for (i = 0; i < A->n; i++)
        for (j = 0; j < B->n; j++) {
            uint64_t x = A->per[i], y = B->per[j];
            a836_add(&R, x / a836_g(x, y) * y, A->cnt[i] * B->cnt[j]);
        }
    return R;
}

/* period distribution of a p-primary block (degree d, order e) with partition lam */
static a836_dist a836_pdist(const int *lam, int len, int d, uint64_t e) {
    a836_dist D = a836_new(lam[0] + 1);
    uint64_t prev = 1; int j;
    a836_add(&D, 1, 1);
    for (j = 1; j <= lam[0]; j++) {
        uint64_t cum = (uint64_t)1 << (d * a836_summin(lam, len, j));
        a836_add(&D, e << a836_clog(j), cum - prev);
        prev = cum;
    }
    return D;
}
/* unipotent affine map = unipotent M of type nu on F_2^(k+1), restricted to hyperplane phi=1, phi of height h */
static a836_dist a836_udist(const int *nu, int len, int h) {
    a836_dist D = a836_new(nu[0] + 1);
    uint64_t prev = 0; int j;
    for (j = 1; j <= nu[0]; j++) {
        uint64_t cum = j <= h ? 0 : (uint64_t)1 << (a836_summin(nu, len, j) - 1);
        a836_add(&D, (uint64_t)1 << a836_clog(j), cum - prev);
        prev = cum;
    }
    return D;
}

static void a836_push(a836_plist *L, const int *cur, int len) {
    if (L->n == L->cap) {
        L->cap = L->cap ? 2 * L->cap : 16;
        L->p = (int **)realloc(L->p, sizeof(int *) * (size_t)L->cap);
        L->len = (int *)realloc(L->len, sizeof(int) * (size_t)L->cap);
    }
    L->p[L->n] = (int *)malloc(sizeof(int) * (size_t)(len > 0 ? len : 1));
    if (len > 0) memcpy(L->p[L->n], cur, sizeof(int) * (size_t)len);
    L->len[L->n] = len;
    L->n++;
}
static void a836_gen(a836_plist *L, int rem, int mx, int *cur, int len) {
    int v;
    if (rem == 0) { a836_push(L, cur, len); return; }
    for (v = rem < mx ? rem : mx; v >= 1; v--) { cur[len] = v; a836_gen(L, rem - v, v, cur, len + 1); }
}

typedef struct {
    int np; int *pdeg; uint64_t *pord;
    a836_plist *PL;
    a836_plist *UP;
    int nu_n; a836_dist *ud; int *uq; int *ug1; int *ug2;
    seq_t M0; seq_t S;
} a836_ctx;

static void a836_rec(a836_ctx *X, int i, int rem, const a836_dist *D, seq_t C) {
    int s, q, d;
    if (rem == 0) {
        int u, t;
        for (u = 0; u < X->nu_n; u++) {
            a836_dist T = a836_conv(&X->ud[u], D);
            uint64_t c = 0, ce = 0, c3 = 0;
            int uq = X->uq[u];
            seq_t W, coef, uc, uw;
            for (t = 0; t < T.n; t++) {
                uint64_t cy = T.cnt[t] / T.per[t];
                c += cy;
                if (T.per[t] % 2 == 0) ce += cy;
                if (T.per[t] % 3 == 0) c3 += cy;
            }
            W = a836_pw2(3 * c) + a836_pw2(2 * c + ce) * 3 + a836_pw2(c + 2 * c3) * 2;
            uc = a836_cent(X->UP->p[uq], X->UP->len[uq], 1);
            uw = a836_pw2((uint64_t)X->ug1[u]) - a836_pw2((uint64_t)X->ug2[u]);
            coef = X->M0 * uw / (uc * C);
            X->S += coef * W;
            a836_free(&T);
        }
        return;
    }
    if (i >= X->np || X->pdeg[i] > rem) return;
    a836_rec(X, i + 1, rem, D, C);
    d = X->pdeg[i];
    for (s = 1; s * d <= rem; s++)
        for (q = 0; q < X->PL[s].n; q++) {
            const int *lam = X->PL[s].p[q];
            int len = X->PL[s].len[q];
            a836_dist P = a836_pdist(lam, len, d, X->pord[i]);
            a836_dist ND = a836_conv(D, &P);
            a836_rec(X, i + 1, rem - s * d, &ND, C * a836_cent(lam, len, d));
            a836_free(&P);
            a836_free(&ND);
        }
}

seq_t a(idx_t n) {
    int N = (int)n, w = 0, lg = 0, d, i, k, s, pcap = 0;
    seq_t x = 1, GL = 1, res;
    a836_ctx X;
    int *cur;
    if (n < 1) return 0;
    while (x != 0 && w < 256) { x *= 2; w++; }
    if (w < 256) {
        while ((1 << lg) < w) lg++;
        if (N > lg - 2) return 0;
    }
    for (i = 0; i < N; i++) GL *= a836_pw2((uint64_t)N) - a836_pw2((uint64_t)i);
    X.M0 = a836_pw2((uint64_t)N) * GL;
    X.S = 0;
    X.np = 0; X.pdeg = NULL; X.pord = NULL;
    for (d = 2; d <= N; d++) {
        uint64_t f;
        for (f = ((uint64_t)1 << d) | 1; f < ((uint64_t)2 << d); f += 2) {
            uint64_t g, cr, e;
            int irr = 1;
            for (g = 2; g < ((uint64_t)1 << (d / 2 + 1)); g++)
                if (a836_pmod(f, g) == 0) { irr = 0; break; }
            if (!irr) continue;
            cr = 2; e = 1;
            while (cr != 1) { cr <<= 1; if ((cr >> d) & 1) cr ^= f; e++; }
            if (X.np == pcap) {
                pcap = pcap ? 2 * pcap : 16;
                X.pdeg = (int *)realloc(X.pdeg, sizeof(int) * (size_t)pcap);
                X.pord = (uint64_t *)realloc(X.pord, sizeof(uint64_t) * (size_t)pcap);
            }
            X.pdeg[X.np] = d; X.pord[X.np] = e; X.np++;
        }
    }
    X.PL = (a836_plist *)calloc((size_t)(N + 2), sizeof(a836_plist));
    cur = (int *)malloc(sizeof(int) * (size_t)(N + 2));
    for (s = 1; s <= N + 1; s++) a836_gen(&X.PL[s], s, s, cur, 0);
    for (k = 0; k <= N; k++) {
        a836_plist *P = &X.PL[k + 1];
        int q, h, cap = 0, u;
        a836_dist E0;
        for (q = 0; q < P->n; q++) cap += P->p[q][0];
        X.UP = P;
        X.ud = (a836_dist *)malloc(sizeof(a836_dist) * (size_t)(cap + 1));
        X.uq = (int *)malloc(sizeof(int) * (size_t)(cap + 1));
        X.ug1 = (int *)malloc(sizeof(int) * (size_t)(cap + 1));
        X.ug2 = (int *)malloc(sizeof(int) * (size_t)(cap + 1));
        X.nu_n = 0;
        for (q = 0; q < P->n; q++) {
            const int *nu = P->p[q];
            int len = P->len[q];
            for (h = 0; h < nu[0]; h++) {
                int g1 = 0, g2 = 0, t;
                for (t = 0; t < len; t++) { if (nu[t] > h) g1++; if (nu[t] > h + 1) g2++; }
                if (g1 == g2) continue;
                X.ud[X.nu_n] = a836_udist(nu, len, h);
                X.uq[X.nu_n] = q;
                X.ug1[X.nu_n] = g1;
                X.ug2[X.nu_n] = g2;
                X.nu_n++;
            }
        }
        E0 = a836_new(1);
        a836_add(&E0, 1, 1);
        a836_rec(&X, 0, N - k, &E0, (seq_t)1);
        a836_free(&E0);
        for (u = 0; u < X.nu_n; u++) a836_free(&X.ud[u]);
        free(X.ud); free(X.uq); free(X.ug1); free(X.ug2);
    }
    res = X.S / (X.M0 * 6);
    for (s = 1; s <= N + 1; s++) {
        int q;
        for (q = 0; q < X.PL[s].n; q++) free(X.PL[s].p[q]);
        free(X.PL[s].p); free(X.PL[s].len);
    }
    free(X.PL); free(cur); free(X.pdeg); free(X.pord);
    return res;
}
