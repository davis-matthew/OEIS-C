/*
 * A005664  Denominators of convergents to log_2 3.
 * https://oeis.org/A005664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = c(n)*a(n-1) + a(n-2), a(-1) = 0, a(0) = 1, where c(n) is
 *            the n-th partial quotient of log_2 3 = [1; 1, 1, 2, 2, 3, 1, 5,
 *            ...]. The partial quotients come from exact big fixed-point
 *            bounds log(2) = 2*atanh(1/3), log(3/2) = 2*atanh(1/5), log_2 3 =
 *            1 + log(3/2)/log(2). Euclid runs on both int...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 80,
 *            big any n
 * Helpers:   add_w, cmp_w, lead_w, qstep, sub_w, submul_w, top_w
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/add_w.h"
#include "../utils/cmp_w.h"
#include "../utils/lead_w.h"
#include "../utils/qstep.h"
#include "../utils/sub_w.h"
#include "../utils/submul_w.h"
#include "../utils/top_w.h"

static void add_small(uint32_t *x, uint32_t v, int L) {
    uint64_t c = v;
    for (int i = 0; c && i < L; i++) {
        uint64_t s = (uint64_t)x[i] + c;
        x[i] = (uint32_t)s;
        c = s >> 32;
    }
}

static void divsmall(uint32_t *dst, const uint32_t *src, uint32_t d, int hi) {
    uint64_t r = 0;
    for (int i = hi; i >= 0; i--) {
        r = (r << 32) | src[i];
        dst[i] = (uint32_t)(r / d);
        r %= d;
    }
}

/* s = floor-approx of atanh(1/m) * 2^(32*(W-1)); returns number of terms */
static long atanh_fp(uint32_t *s, uint32_t *t, uint32_t *u, int W, int L, uint32_t m) {
    memset(s, 0, sizeof(uint32_t) * L);
    memset(t, 0, sizeof(uint32_t) * L);
    t[W - 1] = 1;
    int hi = W - 1;
    divsmall(t, t, m, hi);
    long k = 0;
    while ((hi = top_w(t, hi + 1)) >= 0) {
        divsmall(u, t, (uint32_t)(2 * k + 1), hi);
        add_w(s, u, hi + 1, L);
        divsmall(t, t, m * m, hi);
        k++;
    }
    return k;
}

static uint64_t *cf_tab = NULL;
static long cf_cnt = 0;
static long cf_need = 0;

static void compute_cf(long need) {
    long bits = 4 * need + 256;
    int W = (int)(bits / 32) + 2;
    int L = W + 1;
    uint32_t *S3 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *S5 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *u = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *P1 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *Q1 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *P2 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *Q2 = (uint32_t *)calloc(L, sizeof(uint32_t));

    long k3 = atanh_fp(S3, t, u, W, L, 3);
    long k5 = atanh_fp(S5, t, u, W, L, 5);
    add_w(S3, S3, L, L); /* log 2 */
    add_w(S5, S5, L, L); /* log 3/2 */
    uint32_t e = (uint32_t)(8 * (k3 + k5) + 64);

    memcpy(P1, S5, sizeof(uint32_t) * L);
    add_w(P1, S3, L, L);
    add_small(P1, e, L);
    memcpy(Q1, S3, sizeof(uint32_t) * L);
    add_small(Q1, e, L);
    memcpy(P2, P1, sizeof(uint32_t) * L);
    memcpy(Q2, S3, sizeof(uint32_t) * L);

    long cap = 2L * 32 * L + 8;
    uint64_t *cf = (uint64_t *)malloc(sizeof(uint64_t) * cap);
    long cnt = 0;
    uint32_t *p1 = P1, *q1 = Q1, *p2 = P2, *q2 = Q2, *tmp;
    while (cnt < cap) {
        if (top_w(q1, L) < 0 || top_w(q2, L) < 0) break;
        uint64_t a1 = qstep(p1, q1, L);
        uint64_t a2 = qstep(p2, q2, L);
        if (a1 != a2) break;
        cf[cnt++] = a1;
        tmp = p1; p1 = q1; q1 = tmp;
        tmp = p2; p2 = q2; q2 = tmp;
    }
    if (cnt > 0) cnt--;

    free(S3); free(S5); free(t); free(u);
    free(P1); free(Q1); free(P2); free(Q2);
    if (cf_tab) free(cf_tab);
    cf_tab = cf;
    cf_cnt = cnt;
    cf_need = need;
}

seq_t a(idx_t n) {
    while ((idx_t)cf_cnt <= n) {
        long need = (long)n + 2;
        if (need < 2 * cf_need) need = 2 * cf_need;
        if (need < 64) need = 64;
        compute_cf(need);
    }
    seq_t q0 = 0, q1 = 1;
    for (idx_t i = 1; i <= n; i++) {
        seq_t q = (seq_t)cf_tab[i] * q1 + q0;
        q0 = q1;
        q1 = q;
    }
    return q1;
}
