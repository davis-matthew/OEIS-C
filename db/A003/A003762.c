/*
 * A003762  Number of spanning trees with degrees 1 and 3 in D_4 X P_n.
 * https://oeis.org/A003762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   transfer-matrix count of spanning trees with all degrees in
 *            {1,3} in D_4 X P_n, with D_4 the paw graph (triangle t,u,v plus
 *            a pendant w on t; it gives a(1) = 1, a(2) = 4). The minimal
 *            linear recurrence of the transfer-matrix sequence (Faase's
 *            12-term recurrence) is found by Berlekamp-Massey mod 2...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, make_key
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/make_key.h"

#define MAXS 4096
#define KM (2 * MAXS + 16)

static const int EA[4] = {0, 0, 1, 0};
static const int EB[4] = {1, 2, 2, 3};
static const uint64_t PM = 2305843009213693951ULL;

static int nst = 0;
static int skey[MAXS];
static int keyidx[65536];
static int gfin[MAXS];
static int *tsrc = NULL, *tdst = NULL, *tmul = NULL;
static int ntr = 0, captr = 0;
static int fidx[16], fmul[16], nf = 0;
static seq_t va[MAXS], vb[MAXS];

static int uf_find(int *p, int x) {
    while (p[x] != x) { p[x] = p[p[x]]; x = p[x]; }
    return x;
}

static int get_state(int key) {
    if (keyidx[key] < 0) {
        keyidx[key] = nst;
        skey[nst] = key;
        nst++;
    }
    return keyidx[key];
}

static void add_tr(int s, int d, int mul) {
    if (ntr == captr) {
        captr = captr ? 2 * captr : 1024;
        tsrc = (int *)realloc(tsrc, sizeof(int) * captr);
        tdst = (int *)realloc(tdst, sizeof(int) * captr);
        tmul = (int *)realloc(tmul, sizeof(int) * captr);
    }
    tsrc[ntr] = s; tdst[ntr] = d; tmul[ntr] = mul; ntr++;
}

static void build(void) {
    int cnt[MAXS], touched[MAXS];
    memset(keyidx, -1, sizeof keyidx);
    memset(cnt, 0, sizeof cnt);
    nf = 0;
    for (int mask = 0; mask < 16; mask++) {
        int p[8], deg[4] = {0, 0, 0, 0}, root[4], ok = 1;
        for (int i = 0; i < 8; i++) p[i] = i;
        for (int e = 0; e < 4 && ok; e++) if (mask >> e & 1) {
            int ra = uf_find(p, EA[e]), rb = uf_find(p, EB[e]);
            if (ra == rb) ok = 0;
            else { p[ra] = rb; deg[EA[e]]++; deg[EB[e]]++; }
        }
        if (!ok) continue;
        for (int i = 0; i < 4; i++) root[i] = uf_find(p, i);
        int s = get_state(make_key(deg, root)), j;
        for (j = 0; j < nf; j++) if (fidx[j] == s) break;
        if (j == nf) { fidx[nf] = s; fmul[nf] = 0; nf++; }
        fmul[j]++;
    }
    for (int s = 0; s < nst; s++) {
        int key = skey[s], deg[4], lab[4], nt = 0;
        for (int i = 0; i < 4; i++) {
            deg[i] = key >> (2 * i) & 3;
            lab[i] = key >> (8 + 2 * i) & 3;
        }
        for (int r = 0; r < 16; r++) {
            for (int m = 0; m < 16; m++) {
                int p[8], od[4], nd[4], root[4], ok = 1;
                for (int i = 0; i < 8; i++) p[i] = i;
                for (int i = 1; i < 4; i++)
                    for (int j = 0; j < i; j++)
                        if (lab[j] == lab[i]) { p[i] = j; break; }
                for (int k = 0; k < 4; k++) { od[k] = deg[k]; nd[k] = 0; }
                for (int k = 0; k < 4; k++) if (r >> k & 1) {
                    int ra = uf_find(p, k), rb = uf_find(p, 4 + k);
                    od[k]++; nd[k]++;
                    p[ra] = rb;
                }
                for (int e = 0; e < 4; e++) if (m >> e & 1) {
                    int ra = uf_find(p, 4 + EA[e]), rb = uf_find(p, 4 + EB[e]);
                    if (ra == rb) { ok = 0; break; }
                    p[ra] = rb;
                    nd[EA[e]]++; nd[EB[e]]++;
                }
                if (!ok) continue;
                for (int k = 0; k < 4; k++)
                    if ((od[k] != 1 && od[k] != 3) || nd[k] > 3) ok = 0;
                if (!ok) continue;
                for (int k = 0; k < 4 && ok; k++) {
                    int rk = uf_find(p, k), f = 0;
                    for (int j = 0; j < 4; j++) if (uf_find(p, 4 + j) == rk) f = 1;
                    if (!f) ok = 0;
                }
                if (!ok) continue;
                for (int j = 0; j < 4; j++) root[j] = uf_find(p, 4 + j);
                int d = get_state(make_key(nd, root));
                if (cnt[d]++ == 0) touched[nt++] = d;
            }
        }
        for (int i = 0; i < nt; i++) {
            add_tr(s, touched[i], cnt[touched[i]]);
            cnt[touched[i]] = 0;
        }
    }
    for (int s = 0; s < nst; s++) {
        int key = skey[s], g = 1;
        for (int i = 0; i < 4; i++) {
            int dg = key >> (2 * i) & 3, lb = key >> (8 + 2 * i) & 3;
            if ((dg != 1 && dg != 3) || lb != 0) g = 0;
        }
        gfin[s] = g;
    }
}

static uint64_t addm(uint64_t x, uint64_t y) { uint64_t r = x + y; if (r >= PM) r -= PM; return r; }
static uint64_t subm(uint64_t x, uint64_t y) { return x >= y ? x - y : x + PM - y; }
static uint64_t mulm(uint64_t x, uint64_t y) {
    unsigned __int128 z = (unsigned __int128)x * y;
    uint64_t r = (uint64_t)(z & PM) + (uint64_t)(z >> 61);
    while (r >= PM) r -= PM;
    return r;
}
static uint64_t powm(uint64_t b, uint64_t e) {
    uint64_t r = 1;
    while (e) { if (e & 1) r = mulm(r, b); b = mulm(b, b); e >>= 1; }
    return r;
}

seq_t a(idx_t n) {
    static int init = 0;
    static int L = 0;
    static long long rc[KM];
    static seq_t *cache = NULL;
    static idx_t clen = 0, ccap = 0;

    if (n == 0) return 0;
    if (!init) {
        static uint64_t ta[MAXS], tb[MAXS], terms[KM], C[KM], B[KM], T[KM];
        int lc = 1, lb = 1, m = 1, nt = 0;
        uint64_t bb = 1;
        build();
        memset(ta, 0, sizeof ta);
        for (int i = 0; i < nf; i++) ta[fidx[i]] = addm(ta[fidx[i]], (uint64_t)fmul[i]);
        C[0] = 1; B[0] = 1; L = 0;
        for (;;) {
            uint64_t t = 0, d;
            for (int s = 0; s < nst; s++) if (gfin[s]) t = addm(t, ta[s]);
            terms[nt] = t;
            d = t;
            for (int i = 1; i <= L; i++) d = addm(d, mulm(C[i], terms[nt - i]));
            if (d == 0) {
                m++;
            } else {
                uint64_t coef = mulm(d, powm(bb, PM - 2));
                if (2 * L <= nt) {
                    int tl = lc;
                    memcpy(T, C, sizeof(uint64_t) * tl);
                    for (int i = 0; i < lb; i++) C[i + m] = subm(C[i + m], mulm(coef, B[i]));
                    if (lb + m > lc) lc = lb + m;
                    L = nt + 1 - L;
                    memcpy(B, T, sizeof(uint64_t) * tl);
                    lb = tl; bb = d; m = 1;
                } else {
                    for (int i = 0; i < lb; i++) C[i + m] = subm(C[i + m], mulm(coef, B[i]));
                    if (lb + m > lc) lc = lb + m;
                    m++;
                }
            }
            nt++;
            if ((nt >= 2 * L + 50 && nt >= 64) || nt >= 2 * nst + 2 || nt >= KM - 8) break;
            memset(tb, 0, sizeof(uint64_t) * nst);
            for (int i = 0; i < ntr; i++)
                tb[tdst[i]] = addm(tb[tdst[i]], mulm(ta[tsrc[i]], (uint64_t)tmul[i]));
            memcpy(ta, tb, sizeof(uint64_t) * nst);
        }
        for (int i = 1; i <= L; i++) {
            uint64_t v = C[i] == 0 ? 0 : PM - C[i];
            rc[i] = v > PM / 2 ? -(long long)(PM - v) : (long long)v;
        }
        ccap = (idx_t)L + 16;
        cache = alloc_zero(ccap);
        for (int s = 0; s < nst; s++) va[s] = 0;
        for (int i = 0; i < nf; i++) va[fidx[i]] += (seq_t)fmul[i];
        for (int k = 0; k < L; k++) {
            seq_t t = 0;
            for (int s = 0; s < nst; s++) if (gfin[s]) t += va[s];
            cache[k] = t;
            if (k + 1 < L) {
                for (int s = 0; s < nst; s++) vb[s] = 0;
                for (int i = 0; i < ntr; i++) vb[tdst[i]] += va[tsrc[i]] * (seq_t)tmul[i];
                for (int s = 0; s < nst; s++) va[s] = vb[s];
            }
        }
        clen = (idx_t)L;
        init = 1;
    }
    if (n > ccap) {
        idx_t nc = 2 * ccap > n ? 2 * ccap : n;
        cache = grow_zero(cache, ccap, nc);
        ccap = nc;
    }
    while (clen < n) {
        seq_t pos = 0, neg = 0;
        for (int i = 1; i <= L; i++) {
            if (rc[i] > 0) pos += cache[clen - i] * (seq_t)(uint64_t)rc[i];
            else if (rc[i] < 0) neg += cache[clen - i] * (seq_t)(uint64_t)(-rc[i]);
        }
        cache[clen] = pos - neg;
        clen++;
    }
    return cache[n - 1];
}
