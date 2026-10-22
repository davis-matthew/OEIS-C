/*
 * A002183  Number of divisors of n-th highly composite number.
 * https://oeis.org/A002183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = A000005(A002182(n)). Search: the highly composite numbers
 *            are found one interval at a time, between consecutive superior
 *            highly composite numbers s_k < s_{k+1}. With Ramanujan's epsilon
 *            method, all exponent vectors (non-increasing exponents) whose
 *            deficiency is within a bound are enumerate...
 * Range:     u8 n <= 38, u16 n <= 146, u32 n <= 501, u64 n <= 1692,
 *            u128 n <= 5975, big any n
 * Helpers:   h_bcmp, h_bmul, h_cmpD
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "../utils/h_bcmp.h"
#include "../utils/h_bmul.h"
#include "../utils/h_cmpD.h"

#define H_TOL 1e-9

static uint32_t *h_pr = NULL;
static double *h_lp = NULL;
static size_t h_np = 0, h_pcap = 0;
static double h_lgi[260];
static int h_init = 0;

static void h_primes(size_t k) {
    while (h_np < k) {
        uint32_t c = h_np ? h_pr[h_np - 1] + 1 : 2;
        for (;; c++) {
            int ok = 1;
            size_t i;
            for (i = 0; i < h_np && (uint64_t)h_pr[i] * h_pr[i] <= c; i++)
                if (c % h_pr[i] == 0) { ok = 0; break; }
            if (ok) break;
        }
        if (h_np == h_pcap) {
            h_pcap = h_pcap ? 2 * h_pcap : 256;
            h_pr = (uint32_t *)realloc(h_pr, h_pcap * sizeof(uint32_t));
            h_lp = (double *)realloc(h_lp, h_pcap * sizeof(double));
        }
        h_pr[h_np] = c;
        h_lp[h_np] = log((double)c);
        h_np++;
    }
}

/* exact comparison of prod p_i^u_i and prod p_i^v_i */
static int h_cmpN(const unsigned char *u, const unsigned char *v, int L) {
    double ba = 0, bb = 0;
    int i, k, r;
    size_t ca, cb, na = 1, nb = 1;
    uint32_t *A, *B;
    for (i = 0; i < L; i++) {
        if (u[i] > v[i]) ba += (u[i] - v[i]) * h_lp[i];
        else bb += (v[i] - u[i]) * h_lp[i];
    }
    ca = (size_t)(ba / 22.0) + 3;
    cb = (size_t)(bb / 22.0) + 3;
    A = (uint32_t *)calloc(ca, sizeof(uint32_t));
    B = (uint32_t *)calloc(cb, sizeof(uint32_t));
    A[0] = 1; B[0] = 1;
    for (i = 0; i < L; i++) {
        for (k = u[i]; k > v[i]; k--) na = h_bmul(A, na, h_pr[i]);
        for (k = v[i]; k > u[i]; k--) nb = h_bmul(B, nb, h_pr[i]);
    }
    r = h_bcmp(A, na, B, nb);
    free(A); free(B);
    return r;
}

typedef struct { double x, y; size_t off; } h_cand;
static h_cand *h_cs = NULL;
static size_t h_nc = 0, h_ccap = 0;
static unsigned char *h_pool = NULL;
static size_t h_plcap = 0;
static int h_L, h_E;
static unsigned char *h_est = NULL, *h_cur = NULL, *h_s1 = NULL;
static double *h_T = NULL;
static double h_eps, h_xmax;

static int h_ccmp(const void *p, const void *q) {
    const h_cand *x = (const h_cand *)p, *y = (const h_cand *)q;
    if (x->x < y->x - H_TOL) return -1;
    if (x->x > y->x + H_TOL) return 1;
    return h_cmpN(h_pool + x->off, h_pool + y->off, h_L);
}

static double h_cost(int i, int e) {
    int s = h_est[i];
    return h_lgi[s + 1] - h_lgi[e + 1] - h_eps * (double)(s - e) * h_lp[i];
}

static void h_emit(double x, double y) {
    size_t need;
    if (h_nc == h_ccap) {
        h_ccap = h_ccap ? 2 * h_ccap : 1024;
        h_cs = (h_cand *)realloc(h_cs, h_ccap * sizeof(h_cand));
    }
    need = (h_nc + 1) * (size_t)h_L;
    if (need > h_plcap) {
        h_plcap = need * 2;
        h_pool = (unsigned char *)realloc(h_pool, h_plcap);
    }
    h_cs[h_nc].x = x;
    h_cs[h_nc].y = y;
    h_cs[h_nc].off = h_nc * (size_t)h_L;
    memcpy(h_pool + h_cs[h_nc].off, h_cur, (size_t)h_L);
    h_nc++;
}

static void h_dfs(int i, int cap, double rem, double x, double y) {
    int es, top, e, cc;
    double c;
    if (i == h_L) {
        if (x >= -H_TOL && x <= h_xmax + H_TOL && y >= -H_TOL) h_emit(x, y);
        return;
    }
    es = h_est[i];
    top = es < cap ? es : cap;
    for (e = top; e >= 0; e--) {
        c = h_cost(i, e);
        cc = e < h_E ? e : h_E;
        if (c + h_T[(size_t)(i + 1) * (h_E + 1) + cc] > rem + H_TOL) break;
        h_cur[i] = (unsigned char)e;
        h_dfs(i + 1, e, rem - c, x + (double)(e - es) * h_lp[i],
              y + h_lgi[e + 1] - h_lgi[es + 1]);
    }
    for (e = es + 1; e <= cap; e++) {
        c = h_cost(i, e);
        if (c > rem + H_TOL) break;
        cc = e < h_E ? e : h_E;
        if (c + h_T[(size_t)(i + 1) * (h_E + 1) + cc] > rem + H_TOL) continue;
        h_cur[i] = (unsigned char)e;
        h_dfs(i + 1, e, rem - c, x + (double)(e - es) * h_lp[i],
              y + h_lgi[e + 1] - h_lgi[es + 1]);
    }
}

static unsigned char **h_hv = NULL;
static int *h_hl = NULL;
static size_t h_nh = 0, h_hcap = 0;
static unsigned char *h_se = NULL;
static int h_sm = 0, h_secap = 0;

static void h_push(const unsigned char *v, int len) {
    while (len > 0 && v[len - 1] == 0) len--;
    if (h_nh == h_hcap) {
        h_hcap = h_hcap ? 2 * h_hcap : 256;
        h_hv = (unsigned char **)realloc(h_hv, h_hcap * sizeof(unsigned char *));
        h_hl = (int *)realloc(h_hl, h_hcap * sizeof(int));
    }
    h_hv[h_nh] = (unsigned char *)malloc(len ? (size_t)len : 1);
    if (len) memcpy(h_hv[h_nh], v, (size_t)len);
    h_hl[h_nh] = len;
    h_nh++;
}

static void h_step(void) {
    int m = h_sm, j = 0, L, i, c;
    double best = -1, B, Dmax;
    h_primes((size_t)m + 2);
    for (i = 0; i <= m; i++) {
        int e = i < m ? h_se[i] : 0;
        double ev = (h_lgi[e + 2] - h_lgi[e + 1]) / h_lp[i];
        if (ev > best) { best = ev; j = i; }
    }
    h_eps = best;
    Dmax = best * h_lp[j];
    h_xmax = h_lp[j];
    B = Dmax < 0.004 ? Dmax : 0.004;
    for (;;) {
        int last = B >= Dmax, ok = 1;
        size_t k, nrec = 0, *recs;
        const unsigned char *pv;
        double py = 0;
        L = m + 1;
        h_primes((size_t)L + 1);
        while (h_eps * h_lp[L] - h_lgi[2] <= B + H_TOL) {
            L++;
            h_primes((size_t)L + 1);
        }
        h_L = L;
        h_est = (unsigned char *)realloc(h_est, (size_t)L);
        h_cur = (unsigned char *)realloc(h_cur, (size_t)L);
        h_s1 = (unsigned char *)realloc(h_s1, (size_t)L);
        for (i = 0; i < L; i++) h_est[i] = i < m ? h_se[i] : 0;
        memcpy(h_s1, h_est, (size_t)L);
        h_s1[j]++;
        h_E = m ? h_est[0] : 0;
        h_T = (double *)realloc(h_T, (size_t)(L + 1) * (h_E + 1) * sizeof(double));
        for (c = 0; c <= h_E; c++) h_T[(size_t)L * (h_E + 1) + c] = 0;
        for (i = L - 1; i >= 0; i--)
            for (c = 0; c <= h_E; c++) {
                int e = h_est[i] < c ? h_est[i] : c;
                h_T[(size_t)i * (h_E + 1) + c] =
                    h_T[(size_t)(i + 1) * (h_E + 1) + c] + h_cost(i, e);
            }
        h_nc = 0;
        h_dfs(0, 250, B, 0.0, 0.0);
        qsort(h_cs, h_nc, sizeof(h_cand), h_ccmp);
        recs = (size_t *)malloc((h_nc + 1) * sizeof(size_t));
        pv = h_est;
        for (k = 0; k < h_nc; k++) {
            h_cand *cd = &h_cs[k];
            const unsigned char *v = h_pool + cd->off;
            int gt, top = 0;
            if (cd->x < -H_TOL) continue;
            if (cd->x <= H_TOL && h_cmpN(v, h_est, L) <= 0) continue;
            if (cd->x >= h_xmax - H_TOL) {
                int r = h_cmpN(v, h_s1, L);
                if (r > 0) break;
                if (r == 0) top = 1;
            }
            if (cd->y > py + H_TOL) gt = 1;
            else if (cd->y < py - H_TOL) gt = 0;
            else gt = h_cmpD(v, pv, L) > 0;
            if (!gt) continue;
            if (!last && h_eps * cd->x - py > B) { ok = 0; break; }
            recs[nrec++] = k;
            pv = v;
            py = cd->y;
            if (top) break;
        }
        if (ok) {
            for (k = 0; k < nrec; k++) h_push(h_pool + h_cs[recs[k]].off, L);
            free(recs);
            break;
        }
        free(recs);
        B *= 4;
        if (B > Dmax) B = Dmax;
    }
    if (j == m) {
        if (m + 1 > h_secap) {
            h_secap = h_secap ? 2 * h_secap : 64;
            h_se = (unsigned char *)realloc(h_se, (size_t)h_secap);
        }
        h_se[m] = 0;
        h_sm = m + 1;
    }
    h_se[j]++;
}

seq_t a(idx_t n) {
    seq_t r = 1;
    int i;
    if (!h_init) {
        h_lgi[0] = 0;
        for (i = 1; i < 260; i++) h_lgi[i] = log((double)i);
        h_push(NULL, 0);
        h_init = 1;
    }
    if (n < 1) return 0;
    while (h_nh < n) h_step();
    for (i = 0; i < h_hl[n - 1]; i++)
        r *= (seq_t)(idx_t)(h_hv[n - 1][i] + 1);
    return r;
}
