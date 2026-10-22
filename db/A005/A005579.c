/*
 * A005579  a(n) = smallest number k such that Product_{i=1..k}
 *          prime(i)/(prime(i)-1) > n.
 * https://oeis.org/A005579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k with Sum_{i=1..k} log(prime(i)/(prime(i)-1)) >
 *            log(n). Computed by search: for small n, a segmented sieve
 *            accumulates the sum prime by prime. For large n, Lucy_Hedgehog
 *            prime sums give pi(x), Sum 1/p and Sum 1/p^2 at a Newton-refined
 *            x. A short segmented sieve walk from that x t...
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   a5579_L, a5579_atanh2, a5579_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/a5579_L.h"
#include "../utils/a5579_atanh2.h"
#include "../utils/a5579_isqrt.h"

#define A5579_SEG (1u << 20)

static const long double A5579_LN2 = 0.693147180559945309417232121458176568L;
static const long double A5579_GAMMA = 0.577215664901532860606512090082402431L;
static const long double A5579_EMG = 0.561459483566885169824143214790880784L;
static const long double A5579_Z2 = 1.644934066848226436472415166646025189L;

static long double a5579_ln(long double x) {
    int k = 0;
    while (x >= 2.0L) { x *= 0.5L; k++; }
    while (x < 1.0L) { x *= 2.0L; k--; }
    return k * A5579_LN2 + a5579_atanh2((x - 1) / (x + 1));
}

static long double a5579_exp(long double y) {
    long k = (long)(y / A5579_LN2);
    long double r, s = 1, t = 1;
    int j;
    if ((long double)k * A5579_LN2 > y) k--;
    r = y - (long double)k * A5579_LN2;
    for (j = 1; j < 60; j++) {
        t *= r / j;
        s += t;
        if (t <= s * 1e-22L) break;
    }
    while (k > 0) { s *= 2; k--; }
    while (k < 0) { s *= 0.5L; k++; }
    return s;
}

static long double a5579_H(uint64_t v) {
    long double x, ix, ix2;
    if (v <= 64) {
        long double s = 0;
        uint64_t m;
        for (m = v; m >= 1; m--) s += 1.0L / (long double)m;
        return s;
    }
    x = (long double)v; ix = 1 / x; ix2 = ix * ix;
    return a5579_ln(x) + A5579_GAMMA + ix / 2 - ix2 / 12 + ix2 * ix2 / 120
           - ix2 * ix2 * ix2 / 252 + ix2 * ix2 * ix2 * ix2 / 240;
}

static long double a5579_H2(uint64_t v) {
    long double x, ix, ix2;
    if (v <= 64) {
        long double s = 0;
        uint64_t m;
        for (m = v; m >= 1; m--) s += 1.0L / ((long double)m * (long double)m);
        return s;
    }
    x = (long double)v; ix = 1 / x; ix2 = ix * ix;
    return A5579_Z2 - (ix - ix2 / 2 + ix2 * ix / 6 - ix2 * ix2 * ix / 30
                       + ix2 * ix2 * ix2 * ix / 42 - ix2 * ix2 * ix2 * ix2 * ix / 30);
}

/* F = Sum_{p<=x} log(p/(p-1)), P = pi(x) */
static void a5579_lucy(uint64_t x, long double *F, uint64_t *P) {
    uint64_t r = a5579_isqrt(x), v, i, p;
    int64_t *lc = (int64_t *)malloc((size_t)(r + 1) * sizeof(int64_t));
    int64_t *hc = (int64_t *)malloc((size_t)(r + 1) * sizeof(int64_t));
    long double *l1 = (long double *)malloc((size_t)(r + 1) * sizeof(long double));
    long double *l2 = (long double *)malloc((size_t)(r + 1) * sizeof(long double));
    long double *h1 = (long double *)malloc((size_t)(r + 1) * sizeof(long double));
    long double *h2 = (long double *)malloc((size_t)(r + 1) * sizeof(long double));
    long double small = 0, lr, tail3;

    lc[0] = 0; l1[0] = 0; l2[0] = 0;
    hc[0] = 0; h1[0] = 0; h2[0] = 0;
    for (v = 1; v <= r; v++) {
        lc[v] = (int64_t)v - 1;
        l1[v] = a5579_H(v) - 1;
        l2[v] = a5579_H2(v) - 1;
    }
    for (i = 1; i <= r; i++) {
        uint64_t w = x / i;
        hc[i] = (int64_t)w - 1;
        h1[i] = a5579_H(w) - 1;
        h2[i] = a5579_H2(w) - 1;
    }
    for (p = 2; p <= r; p++) {
        int64_t cp;
        long double c1, c2, fp, f2;
        uint64_t p2, iend;
        if (lc[p] == lc[p - 1]) continue;
        small += a5579_L(p);
        cp = lc[p - 1]; c1 = l1[p - 1]; c2 = l2[p - 1];
        fp = 1.0L / (long double)p; f2 = fp * fp;
        p2 = p * p;
        iend = x / p2;
        if (iend > r) iend = r;
        for (i = 1; i <= iend; i++) {
            uint64_t d = i * p;
            if (d <= r) {
                hc[i] -= hc[d] - cp;
                h1[i] -= fp * (h1[d] - c1);
                h2[i] -= f2 * (h2[d] - c2);
            } else {
                uint64_t w = x / d;
                hc[i] -= lc[w] - cp;
                h1[i] -= fp * (l1[w] - c1);
                h2[i] -= f2 * (l2[w] - c2);
            }
        }
        for (v = r; v >= p2; v--) {
            uint64_t w = v / p;
            lc[v] -= lc[w] - cp;
            l1[v] -= fp * (l1[w] - c1);
            l2[v] -= f2 * (l2[w] - c2);
        }
    }
    lr = a5579_ln((long double)r);
    tail3 = 1.0L / (6.0L * (long double)r * (long double)r * lr);
    *F = small + (h1[1] - l1[r]) + (h2[1] - l2[r]) / 2 + tail3;
    *P = (uint64_t)hc[1];
    free(lc); free(hc); free(l1); free(l2); free(h1); free(h2);
}

static uint32_t *a5579_bp = NULL;
static size_t a5579_nbp = 0;
static uint64_t a5579_bpl = 0;
static unsigned char *a5579_mark = NULL;

static void a5579_ensure_base(uint64_t lim) {
    uint64_t nl, i, j;
    unsigned char *s;
    size_t cnt = 0;
    if (a5579_bpl >= lim) return;
    nl = lim;
    if (nl < 2 * a5579_bpl) nl = 2 * a5579_bpl;
    if (nl < 65536) nl = 65536;
    s = (unsigned char *)calloc((size_t)nl + 1, 1);
    for (i = 2; i * i <= nl; i++)
        if (!s[i])
            for (j = i * i; j <= nl; j += i) s[j] = 1;
    for (i = 2; i <= nl; i++)
        if (!s[i]) cnt++;
    free(a5579_bp);
    a5579_bp = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    a5579_nbp = 0;
    for (i = 2; i <= nl; i++)
        if (!s[i]) a5579_bp[a5579_nbp++] = (uint32_t)i;
    free(s);
    a5579_bpl = nl;
}

static void a5579_seg_sieve(uint64_t lo, uint64_t hi) {
    size_t k;
    if (!a5579_mark) a5579_mark = (unsigned char *)malloc(A5579_SEG);
    memset(a5579_mark, 0, (size_t)(hi - lo));
    a5579_ensure_base(a5579_isqrt(hi) + 1);
    for (k = 0; k < a5579_nbp; k++) {
        uint64_t q = a5579_bp[k], start = q * q, j;
        if (start >= hi) break;
        if (start < lo) start = (lo + q - 1) / q * q;
        for (j = start; j < hi; j += q) a5579_mark[j - lo] = 1;
    }
    for (k = 0; k < 2; k++)
        if ((uint64_t)k >= lo && (uint64_t)k < hi) a5579_mark[k - lo] = 1;
}

static uint64_t a5579_forward(uint64_t s, long double F, uint64_t cnt, long double T) {
    uint64_t lo = s + 1;
    for (;;) {
        uint64_t hi = lo + A5579_SEG, i;
        a5579_seg_sieve(lo, hi);
        for (i = 0; i < hi - lo; i++)
            if (!a5579_mark[i]) {
                F += a5579_L(lo + i);
                cnt++;
                if (F > T) return cnt;
            }
        lo = hi;
    }
}

static uint64_t a5579_backward(uint64_t s, long double F, uint64_t cnt, long double T) {
    uint64_t hi = s + 1;
    for (;;) {
        uint64_t lo = hi > A5579_SEG ? hi - A5579_SEG : 0, i;
        a5579_seg_sieve(lo, hi);
        for (i = hi - lo; i >= 1; i--)
            if (!a5579_mark[i - 1]) {
                long double Fp = F - a5579_L(lo + i - 1);
                if (Fp <= T) return cnt;
                F = Fp;
                cnt--;
            }
        if (lo == 0) return cnt;
        hi = lo;
    }
}

#define A5579_MEMO 128
static uint64_t a5579_memo[A5579_MEMO];
static bool a5579_have[A5579_MEMO];

seq_t a(idx_t n) {
    long double T, lx0, x0, F;
    uint64_t res, P, x;
    int it;
    if (n < 5) {
        static const uint64_t pr[5] = {2, 3, 5, 7, 11};
        uint64_t num = 1, den = 1, k = 0;
        while (!(num > (uint64_t)n * den)) {
            num *= pr[k];
            den *= pr[k] - 1;
            k++;
        }
        return (seq_t)k;
    }
    if (n < A5579_MEMO && a5579_have[n]) return (seq_t)a5579_memo[n];
    T = a5579_ln((long double)n);
    lx0 = (long double)n * A5579_EMG;
    if (lx0 > 42.0L) return 0;
    x0 = a5579_exp(lx0);
    if (x0 < 3e7L) {
        res = a5579_forward(0, 0, 0, T);
    } else {
        x = (uint64_t)x0;
        for (it = 0;; it++) {
            long double xn, diff;
            a5579_lucy(x, &F, &P);
            xn = (long double)x * a5579_exp((T - F) * a5579_ln((long double)x));
            diff = xn - (long double)x;
            if (diff < 0) diff = -diff;
            if (diff < 3e7L || it >= 4) break;
            if (xn < 1e7L) xn = 1e7L;
            if (xn > 1e18L) xn = 1e18L;
            x = (uint64_t)xn;
        }
        if (F > T) res = a5579_backward(x, F, P, T);
        else res = a5579_forward(x, F, P, T);
    }
    if (n < A5579_MEMO) {
        a5579_memo[n] = res;
        a5579_have[n] = true;
    }
    return (seq_t)res;
}
