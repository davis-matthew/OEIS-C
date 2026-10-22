/*
 * A000892  Number of switching networks with GL(n,2) acting on the domain and
 *          AG(2,2) acting on the range.
 * https://oeis.org/A000892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(24*|GL(n,2)|)) * Sum_{g in GL(n,2)} (4^c(g) +
 *            6*4^ce(g)*2^co(g) + 8*4^c3(g)), Burnside count for GL(n,2) x
 *            AG(2,2) acting on maps F_2^n -> F_2^2. Here c, ce, co and c3 are
 *            the numbers of all, even-length, odd-length and
 *            3-divisible-length cycles of g on F_2^n. The sum runs over
 *            conjuga...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   x892_addsh, x892_divs, x892_gcd, x892_muls, x892_pdeg,
 *            x892_pmod, x892_shr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/x892_addsh.h"
#include "../utils/x892_divs.h"
#include "../utils/x892_gcd.h"
#include "../utils/x892_muls.h"
#include "../utils/x892_pdeg.h"
#include "../utils/x892_pmod.h"
#include "../utils/x892_shr.h"

typedef struct { uint64_t o, c; } x892_oc;

#define X892_CAP 16384

static int x892_np, x892_L, x892_TL, x892_GLn;
static int *x892_deg;
static uint64_t *x892_ord;
static uint32_t *x892_S, *x892_T, *x892_W, *x892_G;
static x892_oc *x892_dist[66];
static int x892_dc[66];
static uint64_t x892_E[66];
static int x892_nf[66];
static uint32_t x892_fac[512];
static int x892_part[66][66];

static void x892_leaf(int depth) {
    x892_oc *D = x892_dist[depth];
    int k = x892_dc[depth], i, j, wl, tl;
    uint64_t C = 0, Ce = 0, Co = 0, C3 = 0;
    for (i = 0; i < k; i++) {
        uint64_t cy = D[i].c / D[i].o;
        C += cy;
        if (D[i].o % 2 == 0) Ce += cy; else Co += cy;
        if (D[i].o % 3 == 0) C3 += cy;
    }
    memset(x892_T, 0, sizeof(uint32_t) * (size_t)x892_TL);
    x892_addsh(x892_T, x892_TL, 1, 2 * C);
    x892_addsh(x892_T, x892_TL, 6, 2 * Ce + Co);
    x892_addsh(x892_T, x892_TL, 8, 2 * C3);
    memcpy(x892_W, x892_G, sizeof(uint32_t) * (size_t)x892_GLn);
    x892_shr(x892_W, x892_GLn, x892_E[depth]);
    for (i = 0; i < x892_nf[depth]; i++) x892_divs(x892_W, x892_GLn, x892_fac[i]);
    wl = x892_GLn;
    while (wl > 0 && !x892_W[wl - 1]) wl--;
    tl = x892_TL;
    while (tl > 0 && !x892_T[tl - 1]) tl--;
    for (i = 0; i < wl; i++) {
        uint64_t carry = 0;
        int idx;
        if (!x892_W[i]) continue;
        for (j = 0; j < tl; j++) {
            uint64_t t;
            idx = i + j;
            t = (uint64_t)x892_W[i] * x892_T[j] + x892_S[idx] + carry;
            x892_S[idx] = (uint32_t)t;
            carry = t >> 32;
        }
        idx = i + tl;
        while (carry && idx < x892_L) {
            uint64_t t = (uint64_t)x892_S[idx] + carry;
            x892_S[idx] = (uint32_t)t;
            carry = t >> 32;
            idx++;
        }
    }
}

static void x892_rec(int idx, int rem, int depth);

static void x892_apply(int pi, int rem, int depth, int m, int k) {
    int d = x892_deg[pi];
    uint64_t od = x892_ord[pi];
    int *p = x892_part[depth];
    int lmax = p[0], j, i, a, nc = 0, nf;
    x892_oc *src = x892_dist[depth], *dst = x892_dist[depth + 1];
    int sc = x892_dc[depth];
    uint64_t sprev = 0, ex;
    for (j = 0; j <= lmax; j++) {
        uint64_t sj = 0, cnt, oj;
        for (i = 0; i < k; i++) sj += (uint64_t)(p[i] < j ? p[i] : j);
        sj *= (uint64_t)d;
        if (j == 0) { cnt = 1; oj = 1; }
        else {
            int t = 0;
            while ((1 << t) < j) t++;
            cnt = (1ULL << sj) - (1ULL << sprev);
            oj = od << t;
        }
        sprev = sj;
        for (a = 0; a < sc; a++) {
            uint64_t o = src[a].o / x892_gcd(src[a].o, oj) * oj;
            uint64_t c = src[a].c * cnt;
            int b;
            for (b = 0; b < nc; b++) if (dst[b].o == o) break;
            if (b < nc) dst[b].c += c;
            else if (nc < X892_CAP) { dst[nc].o = o; dst[nc].c = c; nc++; }
        }
    }
    x892_dc[depth + 1] = nc;
    ex = 0;
    for (i = 0; i < k; i++) ex += (uint64_t)(2 * i + 1) * (uint64_t)p[i];
    nf = x892_nf[depth];
    i = 0;
    while (i < k) {
        int mm = 0, kk;
        a = i;
        while (a < k && p[a] == p[i]) { a++; mm++; }
        ex -= (uint64_t)mm * (uint64_t)(mm + 1) / 2;
        for (kk = 1; kk <= mm; kk++) x892_fac[nf++] = (uint32_t)((1ULL << (d * kk)) - 1);
        i = a;
    }
    x892_E[depth + 1] = x892_E[depth] + ex * (uint64_t)d;
    x892_nf[depth + 1] = nf;
    x892_rec(pi + 1, rem - d * m, depth + 1);
}

static void x892_parts(int pi, int rem, int depth, int m, int left, int maxp, int k) {
    int p;
    if (left == 0) { x892_apply(pi, rem, depth, m, k); return; }
    for (p = (left < maxp ? left : maxp); p >= 1; p--) {
        x892_part[depth][k] = p;
        x892_parts(pi, rem, depth, m, left - p, p, k + 1);
    }
}

static void x892_rec(int idx, int rem, int depth) {
    int i, m;
    if (rem == 0) { x892_leaf(depth); return; }
    for (i = idx; i < x892_np && x892_deg[i] <= rem; i++)
        for (m = 1; m * x892_deg[i] <= rem; m++)
            x892_parts(i, rem, depth, m, m, m, 0);
}

static seq_t x892_compute(int n) {
    uint64_t *polys;
    int cap = 16, d, i, hi;
    uint64_t f, half = (uint64_t)n * (uint64_t)(n - 1) / 2;
    seq_t r = 0;

    polys = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)cap);
    x892_deg = (int *)malloc(sizeof(int) * (size_t)cap);
    x892_ord = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)cap);
    x892_np = 0;
    for (d = 1; d <= n; d++) {
        for (f = (1ULL << d) | 1ULL; f < (2ULL << d); f += 2) {
            int ok = 1;
            for (i = 0; i < x892_np && 2 * x892_deg[i] <= d; i++)
                if (x892_pmod(f, polys[i]) == 0) { ok = 0; break; }
            if (!ok) continue;
            if (x892_np == cap) {
                cap *= 2;
                polys = (uint64_t *)realloc(polys, sizeof(uint64_t) * (size_t)cap);
                x892_deg = (int *)realloc(x892_deg, sizeof(int) * (size_t)cap);
                x892_ord = (uint64_t *)realloc(x892_ord, sizeof(uint64_t) * (size_t)cap);
            }
            {
                uint64_t rr = x892_pmod(2, f), e = 1;
                while (rr != 1) {
                    rr <<= 1;
                    if (rr >> d & 1) rr ^= f;
                    e++;
                }
                polys[x892_np] = f;
                x892_deg[x892_np] = d;
                x892_ord[x892_np] = e;
                x892_np++;
            }
        }
    }

    x892_GLn = (n * n) / 32 + 2;
    x892_TL = (int)(((2ULL << n) + 64) / 32) + 1;
    x892_L = x892_TL + x892_GLn + 2;
    x892_S = (uint32_t *)calloc((size_t)x892_L, sizeof(uint32_t));
    x892_T = (uint32_t *)calloc((size_t)x892_TL, sizeof(uint32_t));
    x892_W = (uint32_t *)calloc((size_t)x892_GLn, sizeof(uint32_t));
    x892_G = (uint32_t *)calloc((size_t)x892_GLn, sizeof(uint32_t));
    x892_addsh(x892_G, x892_GLn, 1, half);
    for (i = 1; i <= n; i++) x892_muls(x892_G, x892_GLn, (uint32_t)((1ULL << i) - 1));

    for (i = 0; i <= n + 1; i++) x892_dist[i] = (x892_oc *)malloc(sizeof(x892_oc) * X892_CAP);
    x892_dist[0][0].o = 1;
    x892_dist[0][0].c = 1;
    x892_dc[0] = 1;
    x892_E[0] = 0;
    x892_nf[0] = 0;
    x892_rec(0, n, 0);

    x892_divs(x892_S, x892_L, 24);
    x892_shr(x892_S, x892_L, half);
    for (i = 1; i <= n; i++) x892_divs(x892_S, x892_L, (uint32_t)((1ULL << i) - 1));

    hi = x892_L - 1;
    while (hi > 0 && !x892_S[hi]) hi--;
    for (i = hi; i >= 0; i--) {
        int b;
        for (b = 3; b >= 0; b--) {
            r *= 256;
            r += (x892_S[i] >> (8 * b)) & 0xffu;
        }
    }

    for (i = 0; i <= n + 1; i++) free(x892_dist[i]);
    free(x892_S); free(x892_T); free(x892_W); free(x892_G);
    free(polys); free(x892_deg); free(x892_ord);
    return r;
}

seq_t a(idx_t n) {
    static seq_t memo[32];
    static int have[32];
    int k;
    if (n < 1 || n > 31) return 0;
    k = (int)n;
    if (!have[k]) {
        memo[k] = x892_compute(k);
        have[k] = 1;
    }
    return memo[k];
}
