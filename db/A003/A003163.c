/*
 * A003163  Denominators of Van der Pol numbers.
 * https://oeis.org/A003163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, nice, easy
 * Formula:   a(n) = denominator of f(n), where f(0) = 1 and f(n) =
 *            -Sum_{k=0..n-1} binomial(n+3,k)*(n+1-k)*f(k) / binomial(n+3,3)
 *            (from F(x)*(6x(e^x+1)-12(e^x-1)) = x^3, F = e.g.f. of f). The
 *            f(k) are kept as numerators over a common denominator stored in
 *            factored form. Binomial sums come from a Pascal antidi...
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 87,
 *            big any n
 * Helpers:   vdp_add, vdp_div, vdp_factor, vdp_isneg, vdp_mod, vdp_muls,
 *            vdp_neg, vdp_putf, vdp_sub, vdp_trim, vdp_used
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/vdp_add.h"
#include "../utils/vdp_div.h"
#include "../utils/vdp_factor.h"
#include "../utils/vdp_isneg.h"
#include "../utils/vdp_mod.h"
#include "../utils/vdp_muls.h"
#include "../utils/vdp_neg.h"
#include "../utils/vdp_putf.h"
#include "../utils/vdp_sub.h"
#include "../utils/vdp_trim.h"
#include "../utils/vdp_used.h"

static uint32_t *vdp_ad = NULL;
static size_t vdp_W = 0, vdp_adcap = 0;
static idx_t vdp_done = 0;
static uint32_t *vdp_E = NULL;
static size_t vdp_Ecap = 0;
static uint32_t *vdp_dp = NULL, *vdp_de = NULL;
static size_t vdp_dlen = 0, vdp_dcap = 0;
static size_t *vdp_off = NULL;
static size_t vdp_offcap = 0;

static void vdp_widen(size_t nw, size_t cnt) {
    uint32_t *na = (uint32_t *)malloc(vdp_adcap * nw * sizeof(uint32_t));
    size_t j, i;
    for (j = 0; j < cnt; j++) {
        uint32_t *src = vdp_ad + j * vdp_W, *dst = na + j * nw;
        uint32_t fill = (src[vdp_W - 1] >> 31) ? 0xFFFFFFFFu : 0u;
        memcpy(dst, src, vdp_W * sizeof(uint32_t));
        for (i = vdp_W; i < nw; i++) dst[i] = fill;
    }
    free(vdp_ad);
    vdp_ad = na;
    vdp_W = nw;
}
static void vdp_ensure_ad(size_t entries) {
    if (entries > vdp_adcap) {
        size_t nc = vdp_adcap * 2;
        if (nc < entries) nc = entries + 16;
        vdp_ad = (uint32_t *)realloc(vdp_ad, nc * vdp_W * sizeof(uint32_t));
        vdp_adcap = nc;
    }
}

static void vdp_push_den(uint32_t p, uint32_t d) {
    if (vdp_dlen >= vdp_dcap) {
        vdp_dcap = vdp_dcap * 2 + 64;
        vdp_dp = (uint32_t *)realloc(vdp_dp, vdp_dcap * sizeof(uint32_t));
        vdp_de = (uint32_t *)realloc(vdp_de, vdp_dcap * sizeof(uint32_t));
    }
    vdp_dp[vdp_dlen] = p; vdp_de[vdp_dlen] = d; vdp_dlen++;
}

static void vdp_step(size_t n) {
    size_t w, m, j, L, i, mx = 0;
    uint32_t *buf, *a1, *a2, *a3, *s4, *t, *S, *mg;
    uint64_t ps[64]; uint32_t es[64]; int np = 0, q;
    uint32_t mults[128]; int nm = 0; uint64_t cur = 1;
    int neg;

    for (j = 0; j < n; j++) {
        size_t u = vdp_used(vdp_ad + j * vdp_W, vdp_W);
        if (u > mx) mx = u;
    }
    if (mx + 6 > vdp_W) vdp_widen(mx + 12, n);
    vdp_ensure_ad(n + 1);
    if (vdp_Ecap < n + 8) {
        size_t nc = (n + 8) * 2;
        vdp_E = (uint32_t *)realloc(vdp_E, nc * sizeof(uint32_t));
        for (i = vdp_Ecap; i < nc; i++) vdp_E[i] = 0;
        vdp_Ecap = nc;
    }
    w = vdp_W;
    buf = (uint32_t *)calloc(7 * w, sizeof(uint32_t));
    a1 = buf; a2 = buf + w; a3 = buf + 2 * w; s4 = buf + 3 * w;
    t = buf + 4 * w; S = buf + 5 * w; mg = buf + 6 * w;

    for (m = 0; m <= n + 2; m++) {
        vdp_add(s4, a3, w);
        if (m <= n + 1) {
            vdp_add(a3, a2, w);
            if (m <= n) {
                uint32_t *e = vdp_ad + m * w;
                if (m < n) {
                    memcpy(t, e, w * sizeof(uint32_t));
                    memcpy(e, a1, w * sizeof(uint32_t));
                    vdp_add(a1, t, w);
                } else {
                    memcpy(e, a1, w * sizeof(uint32_t));
                }
                vdp_add(a2, e, w);
            }
        }
    }
    memcpy(S, a3, w * sizeof(uint32_t));
    vdp_muls(S, w, (uint32_t)(n + 3));
    vdp_sub(S, s4, w);
    vdp_sub(S, s4, w);

    neg = vdp_isneg(S, w);
    memcpy(mg, S, w * sizeof(uint32_t));
    if (neg) vdp_neg(mg, w);
    L = vdp_trim(mg, w);

    vdp_factor(ps, es, &np, (uint64_t)n + 1);
    vdp_factor(ps, es, &np, (uint64_t)n + 2);
    vdp_factor(ps, es, &np, (uint64_t)n + 3);
    for (q = 0; q < np; q++) if (ps[q] == 2 || ps[q] == 3) es[q]--;

    for (q = 0; q < np; q++) {
        uint32_t p = (uint32_t)ps[q], e = es[q], k = 0, re;
        while (k < e && vdp_mod(mg, L, p) == 0) {
            vdp_div(mg, L, p);
            L = vdp_trim(mg, L);
            k++;
        }
        re = e - k;
        vdp_E[p] += re;
        while (re-- > 0) {
            if (cur * p >= ((uint64_t)1 << 32)) { mults[nm++] = (uint32_t)cur; cur = p; }
            else cur *= p;
        }
    }
    if (cur > 1) mults[nm++] = (uint32_t)cur;

    memcpy(t, mg, w * sizeof(uint32_t));
    if (!neg) vdp_neg(t, w);
    for (j = 0; j <= n; j++) {
        uint32_t *e = vdp_ad + j * w;
        for (q = 0; q < nm; q++) vdp_muls(e, w, mults[q]);
        vdp_add(e, t, w);
    }

    if (vdp_offcap < n + 2) {
        vdp_offcap = (n + 2) * 2;
        vdp_off = (size_t *)realloc(vdp_off, vdp_offcap * sizeof(size_t));
    }
    vdp_off[n] = vdp_dlen;
    if (L > 0) {
        uint32_t p;
        for (p = 2; p <= (uint32_t)(n + 3); p++) {
            uint32_t E = vdp_E[p], d;
            if (E == 0) continue;
            if (vdp_mod(mg, L, p) != 0) d = E;
            else {
                size_t Lt = L;
                uint32_t v = 0;
                memcpy(a1, mg, L * sizeof(uint32_t));
                while (v < E && vdp_mod(a1, Lt, p) == 0) {
                    vdp_div(a1, Lt, p);
                    Lt = vdp_trim(a1, Lt);
                    v++;
                }
                d = E - v;
            }
            if (d > 0) vdp_push_den(p, d);
        }
    }
    vdp_off[n + 1] = vdp_dlen;
    free(buf);
}

seq_t a(idx_t n) {
    seq_t r = 1;
    size_t i;
    uint32_t k;
    if (vdp_done == 0) {
        vdp_W = 8;
        vdp_adcap = 16;
        vdp_ad = (uint32_t *)calloc(vdp_adcap * vdp_W, sizeof(uint32_t));
        vdp_ad[0] = 1;
        vdp_offcap = 16;
        vdp_off = (size_t *)malloc(vdp_offcap * sizeof(size_t));
        vdp_off[0] = 0;
        vdp_off[1] = 0;
        vdp_done = 1;
    }
    while (vdp_done <= n) {
        vdp_step((size_t)vdp_done);
        vdp_done++;
    }
    for (i = vdp_off[n]; i < vdp_off[n + 1]; i++)
        for (k = 0; k < vdp_de[i]; k++) r *= (seq_t)vdp_dp[i];
    return r;
}
