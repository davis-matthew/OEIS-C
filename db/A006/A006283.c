/*
 * A006283  Pierce expansion for 1 / Pi.
 * https://oeis.org/A006283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Pierce expansion of 1/Pi: x_0 = 1/Pi, a(k) = floor(1/x_k),
 *            x_{k+1} = 1 - a(k)*x_k. Pi is computed exactly to fixed
 *            precision (Machin + Euler atan series by binary splitting), the
 *            expansion is run in exact integer arithmetic with rigorous error
 *            bounds, and precision is increased until a(n) is certain
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 16, u64 n <= 35, u128 n <= 78,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"

typedef uint32_t limb;

static int bn_norm(const limb *a, int n) { while (n > 0 && a[n - 1] == 0) n--; return n; }

static int bn_cmp(const limb *a, int na, const limb *b, int nb) {
    if (na != nb) return na < nb ? -1 : 1;
    for (int i = na - 1; i >= 0; i--)
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    return 0;
}

/* out = a + b (out may alias a or b); returns length */
static int bn_add(limb *out, const limb *a, int na, const limb *b, int nb) {
    if (na < nb) { const limb *t = a; a = b; b = t; int tn = na; na = nb; nb = tn; }
    uint64_t c = 0; int i;
    for (i = 0; i < nb; i++) { c += (uint64_t)a[i] + b[i]; out[i] = (limb)c; c >>= 32; }
    for (; i < na; i++) { c += a[i]; out[i] = (limb)c; c >>= 32; }
    if (c) out[i++] = (limb)c;
    return i;
}

/* out = a * b (out must not alias); returns length */
static int bn_mul(limb *out, const limb *a, int na, const limb *b, int nb) {
    if (!na || !nb) return 0;
    memset(out, 0, sizeof(limb) * (size_t)(na + nb));
    for (int i = 0; i < na; i++) {
        uint64_t ai = a[i], c = 0;
        if (!ai) continue;
        for (int j = 0; j < nb; j++) {
            c += ai * b[j] + out[i + j];
            out[i + j] = (limb)c; c >>= 32;
        }
        out[i + nb] = (limb)c;
    }
    return bn_norm(out, na + nb);
}

/* q = u / v, r = u % v (Knuth D); u, v normalized, n >= 1. returns length of r */
static int bn_divmod(limb *q, int *nq, limb *r, const limb *u, int m,
                     const limb *v, int n, limb *un, limb *vn) {
    if (m < n) { memcpy(r, u, sizeof(limb) * (size_t)m); *nq = 0; return m; }
    if (n == 1) {
        uint64_t k = 0;
        for (int j = m - 1; j >= 0; j--) {
            uint64_t cur = (k << 32) | u[j];
            q[j] = (limb)(cur / v[0]); k = cur % v[0];
        }
        *nq = bn_norm(q, m); r[0] = (limb)k;
        return bn_norm(r, 1);
    }
    int s = 0; limb top = v[n - 1];
    while (!(top & 0x80000000u)) { top <<= 1; s++; }
    for (int i = n - 1; i > 0; i--) vn[i] = (v[i] << s) | (s ? v[i - 1] >> (32 - s) : 0);
    vn[0] = v[0] << s;
    un[m] = s ? u[m - 1] >> (32 - s) : 0;
    for (int i = m - 1; i > 0; i--) un[i] = (u[i] << s) | (s ? u[i - 1] >> (32 - s) : 0);
    un[0] = u[0] << s;
    for (int j = m - n; j >= 0; j--) {
        uint64_t num = ((uint64_t)un[j + n] << 32) | un[j + n - 1];
        uint64_t qhat = num / vn[n - 1], rhat = num % vn[n - 1];
        while ((qhat >> 32) || qhat * vn[n - 2] > ((rhat << 32) | un[j + n - 2])) {
            qhat--; rhat += vn[n - 1];
            if (rhat >> 32) break;
        }
        uint64_t carry = 0, borrow = 0;
        for (int i = 0; i < n; i++) {
            uint64_t p = qhat * vn[i] + carry;
            carry = p >> 32;
            uint64_t sub = (uint64_t)un[i + j] - (p & 0xFFFFFFFFu) - borrow;
            un[i + j] = (limb)sub;
            borrow = sub >> 63;
        }
        uint64_t sub = (uint64_t)un[j + n] - carry - borrow;
        un[j + n] = (limb)sub;
        if (sub >> 63) {
            qhat--;
            uint64_t c = 0;
            for (int i = 0; i < n; i++) {
                c += (uint64_t)un[i + j] + vn[i];
                un[i + j] = (limb)c; c >>= 32;
            }
            un[j + n] += (limb)c;
        }
        q[j] = (limb)qhat;
    }
    for (int i = 0; i < n; i++)
        r[i] = s ? (un[i] >> s) | (un[i + 1] << (32 - s)) : un[i];
    *nq = bn_norm(q, m - n + 1);
    return bn_norm(r, n);
}

static void sub_into(limb *a, int na, const limb *b, int nb) {
    uint64_t br = 0; int i;
    for (i = 0; i < nb; i++) { uint64_t s = (uint64_t)a[i] - b[i] - br; a[i] = (limb)s; br = s >> 63; }
    for (; br && i < na; i++) { uint64_t s = (uint64_t)a[i] - br; a[i] = (limb)s; br = s >> 63; }
}

static void mulsmall(limb *a, int n, uint32_t m) {
    uint64_t c = 0;
    for (int i = 0; i < n; i++) { c += (uint64_t)a[i] * m; a[i] = (limb)c; c >>= 32; }
}

static limb *small_bn(uint64_t v, int *len) {
    limb *p = (limb *)malloc(sizeof(limb) * 2);
    p[0] = (limb)v; p[1] = (limb)(v >> 32);
    *len = bn_norm(p, 2);
    return p;
}

/* Binary splitting for sum_{k=a}^{b-1} prod_{j=a}^{k} p(j)/q(j), p(j) = j, q(j) = c*(2j+1) */
static void bsplit(int a, int b, uint32_t c, int needP,
                   limb **P, int *nP, limb **Q, int *nQ, limb **T, int *nT) {
    if (b - a == 1) {
        *P = small_bn((uint64_t)a, nP);
        *Q = small_bn((uint64_t)c * (uint64_t)(2 * (uint64_t)a + 1), nQ);
        *T = small_bn((uint64_t)a, nT);
        return;
    }
    int m = (a + b) / 2;
    limb *Pl, *Ql, *Tl, *Pr, *Qr, *Tr;
    int nPl, nQl, nTl, nPr, nQr, nTr;
    bsplit(a, m, c, 1, &Pl, &nPl, &Ql, &nQl, &Tl, &nTl);
    bsplit(m, b, c, needP, &Pr, &nPr, &Qr, &nQr, &Tr, &nTr);
    limb *Qo = (limb *)malloc(sizeof(limb) * (size_t)(nQl + nQr));
    *nQ = bn_mul(Qo, Ql, nQl, Qr, nQr);
    limb *t1 = (limb *)malloc(sizeof(limb) * (size_t)(nTl + nQr));
    int n1 = bn_mul(t1, Tl, nTl, Qr, nQr);
    limb *t2 = (limb *)malloc(sizeof(limb) * (size_t)(nPl + nTr));
    int n2 = bn_mul(t2, Pl, nPl, Tr, nTr);
    limb *To = (limb *)malloc(sizeof(limb) * (size_t)((n1 > n2 ? n1 : n2) + 1));
    *nT = bn_add(To, t1, n1, t2, n2);
    free(t1); free(t2);
    limb *Po = NULL; int nPo = 0;
    if (needP && Pr) {
        Po = (limb *)malloc(sizeof(limb) * (size_t)(nPl + nPr));
        nPo = bn_mul(Po, Pl, nPl, Pr, nPr);
    }
    free(Pl); free(Ql); free(Tl); free(Pr); free(Qr); free(Tr);
    *P = Po; *nP = nPo; *Q = Qo; *T = To;
}

/* out (pre-zeroed) = floor(atan(1/x) * 2^(32*sh)), via Euler's series;
   lg <= log2(1+x^2) */
static void atan_scaled(limb *out, uint32_t x, int sh, int lg) {
    int bits = 32 * sh + 16;
    int K = bits / lg + 2;
    uint32_t c = (1 + x * x) / 2;
    limb *P, *Q, *T; int nP, nQ, nT;
    bsplit(1, K + 1, c, 0, &P, &nP, &Q, &nQ, &T, &nT);
    free(P);
    int mx = nQ > nT ? nQ : nT;
    int m = sh + mx + 2;
    limb *num = (limb *)calloc((size_t)m + 1, sizeof(limb));
    int nqt = bn_add(num + sh, Q, nQ, T, nT);
    mulsmall(num + sh, nqt + 1, x);
    m = bn_norm(num, sh + nqt + 1);
    limb *den = (limb *)calloc((size_t)nQ + 1, sizeof(limb));
    memcpy(den, Q, sizeof(limb) * (size_t)nQ);
    mulsmall(den, nQ + 1, 1 + x * x);
    int nd = bn_norm(den, nQ + 1);
    limb *qq = (limb *)calloc((size_t)m + 1, sizeof(limb));
    limb *rr = (limb *)calloc((size_t)nd + 1, sizeof(limb));
    limb *un = (limb *)calloc((size_t)m + 2, sizeof(limb));
    limb *vn = (limb *)calloc((size_t)nd + 1, sizeof(limb));
    int nq;
    bn_divmod(qq, &nq, rr, num, m, den, nd, un, vn);
    memcpy(out, qq, sizeof(limb) * (size_t)nq);
    free(Q); free(T); free(num); free(den); free(qq); free(rr); free(un); free(vn);
}

static seq_t limbs_to_seq(const limb *q, int nq) {
    seq_t v = 0;
    for (int i = nq - 1; i >= 0; i--) {
        v *= 65536;
        v *= 65536;
        v += (seq_t)q[i];
    }
    return v;
}

static seq_t *cache = NULL;
static idx_t ncache = 0, cap_cache = 0;
static int lastNL = 0;

/* Run with NL limbs of precision; stores all certain terms into cache; returns count */
static idx_t run(int NL) {
    int C = 2 * NL + 8;
    limb *buf = (limb *)calloc((size_t)C * 13, sizeof(limb));
    limb *A = buf, *B = A + C, *Y = B + C, *E = Y + C, *q = E + C, *r = q + C;
    limb *T = r + C, *T2 = T + C, *S = T2 + C, *un = S + C, *vn = un + C;
    limb one[1] = {1}, two[1] = {2};

    atan_scaled(A, 5, NL - 2, 4);
    atan_scaled(B, 239, NL - 2, 15);
    mulsmall(A, NL, 16);
    mulsmall(B, NL, 4);
    sub_into(A, NL, B, NL);
    /* |Pi - pi*2^P| < 2 with P = 32*(NL-3) */
    limb *Pi = A + 1;
    int nPi = bn_norm(Pi, NL - 1);

    Y[NL - 3] = 1;
    int nY = NL - 2, nE = 0;   /* E bounds half the error in Y */
    idx_t k = 0;
    for (;; k++) {
        if (nY == 0) break;
        int nq;
        int nr = bn_divmod(q, &nq, r, Pi, nPi, Y, nY, un, vn);
        int nT = bn_mul(T, q, nq, E, nE);
        int nT2 = bn_add(T2, T, nT, T, nT);
        nT2 = bn_add(T2, T2, nT2, two, 1);
        if (bn_cmp(T2, nT2, r, nr) > 0) break;          /* need 2qE + 2 <= r */
        int nS = bn_add(S, r, nr, T2, nT2);
        nS = bn_add(S, S, nS, E, nE);
        nS = bn_add(S, S, nS, E, nE);
        if (bn_cmp(S, nS, Y, nY) >= 0) break;           /* need r + 2(q+1)E + 2 < Y */
        if (k >= cap_cache) {
            idx_t nc = cap_cache ? cap_cache * 2 : 64;
            cache = grow_zero(cache, cap_cache, nc);
            cap_cache = nc;
        }
        cache[k] = limbs_to_seq(q, nq);
        nE = bn_add(E, T, nT, one, 1);
        memcpy(Y, r, sizeof(limb) * (size_t)nr);
        nY = nr;
    }
    free(buf);
    return k;
}

seq_t a(idx_t n) {
    while (n >= ncache) {
        uint64_t est = (uint64_t)(n + 2) * (uint64_t)(n + 2) * 27 / 1024 + 9;
        uint64_t grow = (uint64_t)lastNL * 13 / 10 + 4;
        int NL = (int)(est > grow ? est : grow);
        lastNL = NL;
        idx_t got = run(NL);
        if (got > ncache) ncache = got;
    }
    return cache[n];
}
