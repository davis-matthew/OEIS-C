/*
 * A005629  Number of achiral trees with n nodes.
 * https://oeis.org/A005629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p(n) + s(n/2) + s((n-1)/4))/2, where s(k) = 0 for
 *            non-integer k; s = A000625: S(x) = 1 + x*(S(x)^3 + 2*S(x^3))/3;
 *            p = A005627: P(x) = 1 + x*P(x)*S(x^2). In fixed width, values
 *            are kept mod 2^(w+1) so that the division by 2 is exact
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

typedef struct { seq_t lo; unsigned hi; } wn;

static int w_ready = 0;
static bool fw;
static unsigned gw, hb;
static seq_t hmask, smax;

static void w_init(void) {
    if (w_ready) return;
    w_ready = 1;
    fw = seq_is_fixed_width();
    if (fw) {
        smax = (seq_t)((seq_t)0 - 1);
        seq_t m = smax;
        gw = 0;
        while (m != 0) { m >>= 1; gw++; }
        hb = gw / 2;
        hmask = (seq_t)(((seq_t)1 << hb) - 1);
    }
}

static wn wmk(seq_t v) { wn r; r.lo = v; r.hi = 0; return r; }

static wn wadd(wn a, wn b) {
    wn r;
    r.lo = (seq_t)(a.lo + b.lo);
    r.hi = 0;
    if (fw) {
        unsigned c = (r.lo < a.lo) ? 1u : 0u;
        r.hi = (a.hi ^ b.hi ^ c) & 1u;
    }
    return r;
}

static wn wmul(wn a, wn b) {
    wn r;
    if (!fw) { r.lo = a.lo * b.lo; r.hi = 0; return r; }
    seq_t a0 = (seq_t)(a.lo & hmask), a1 = (seq_t)(a.lo >> hb);
    seq_t b0 = (seq_t)(b.lo & hmask), b1 = (seq_t)(b.lo >> hb);
    seq_t p00 = (seq_t)(a0 * b0), p01 = (seq_t)(a0 * b1);
    seq_t p10 = (seq_t)(a1 * b0), p11 = (seq_t)(a1 * b1);
    seq_t mid = (seq_t)((seq_t)(p00 >> hb) + (seq_t)(p01 & hmask) + (seq_t)(p10 & hmask));
    seq_t carry = (seq_t)((seq_t)(p01 >> hb) + (seq_t)(p10 >> hb) + (seq_t)(mid >> hb));
    unsigned bit = (unsigned)((seq_t)(p11 + carry) & 1);
    r.lo = (seq_t)(p00 + (seq_t)((seq_t)(p01 + p10) << hb));
    r.hi = (bit + a.hi * (unsigned)(b.lo & 1) + b.hi * (unsigned)(a.lo & 1)) & 1u;
    return r;
}

static wn wdiv3(wn a) {
    if (!fw) return wmk(a.lo / 3);
    /* inverse of 3 modulo 2^(w+1) is (2^(w+1)+1)/3 = 2*((2^w-1)/3)+1 */
    return wmul(a, wmk((seq_t)((seq_t)(smax / 3) * 2 + 1)));
}

static idx_t cap = 0;
static seq_t *tlo = NULL, *plo = NULL;
static unsigned char *thi = NULL, *phi = NULL;
static idx_t tlen = 0, plen = 0;

static wn tget(idx_t i) { wn r; r.lo = tlo[i]; r.hi = thi[i]; return r; }
static wn pget(idx_t i) { wn r; r.lo = plo[i]; r.hi = phi[i]; return r; }

static void build(idx_t newcap) {
    if (tlo) { release(tlo, tlen); free(thi); }
    if (plo) { release(plo, plen); free(phi); }
    tlen = newcap / 2 + 2;
    plen = newcap + 1;
    tlo = alloc_zero(tlen);
    thi = (unsigned char *)calloc(tlen, 1);
    plo = alloc_zero(plen);
    phi = (unsigned char *)calloc(plen, 1);
    seq_t *t2lo = alloc_zero(tlen);
    unsigned char *t2hi = (unsigned char *)calloc(tlen, 1);

    tlo[0] = 1;
    for (idx_t k = 0; k + 1 < tlen; k++) {
        wn s2 = wmk(0);
        for (idx_t i = 0; i <= k; i++) s2 = wadd(s2, wmul(tget(i), tget(k - i)));
        t2lo[k] = s2.lo; t2hi[k] = (unsigned char)s2.hi;
        wn s3 = wmk(0);
        for (idx_t i = 0; i <= k; i++) {
            wn q; q.lo = t2lo[k - i]; q.hi = t2hi[k - i];
            s3 = wadd(s3, wmul(tget(i), q));
        }
        if (k % 3 == 0) {
            wn tt = tget(k / 3);
            s3 = wadd(s3, wadd(tt, tt));
        }
        wn nt = wdiv3(s3);
        tlo[k + 1] = nt.lo; thi[k + 1] = (unsigned char)nt.hi;
    }
    release(t2lo, tlen);
    free(t2hi);

    plo[0] = 1;
    for (idx_t k = 0; k + 1 < plen; k++) {
        wn s = wmk(0);
        for (idx_t l = 0; 2 * l <= k; l++) s = wadd(s, wmul(tget(l), pget(k - 2 * l)));
        plo[k + 1] = s.lo; phi[k + 1] = (unsigned char)s.hi;
    }
    cap = newcap;
}

seq_t a(idx_t n) {
    w_init();
    if (n > cap) {
        idx_t nc = cap * 2;
        if (nc < n) nc = n;
        if (nc < 64) nc = 64;
        build(nc);
    }
    wn x = pget(n);
    if (n % 2 == 0) x = wadd(x, tget(n / 2));
    if ((n - 1) % 4 == 0) x = wadd(x, tget((n - 1) / 4));
    if (!fw) return x.lo / 2;
    return (seq_t)((seq_t)(x.lo >> 1) | (seq_t)((seq_t)x.hi << (gw - 1)));
}
