/*
 * A007507  Decimal expansion of 2^sqrt(2).
 * https://oeis.org/A007507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = n-th digit of 2^sqrt(2) = 2*exp((sqrt(2)-1)*log(2)),
 *            computed in multiprecision fixed point: 1/sqrt(2) by Newton,
 *            log(2) = 2*atanh(1/3), exp by bit-burst splitting of the
 *            argument
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9788 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void divs(uint32_t *a, size_t L, uint32_t d) {
    uint64_t rem = 0;
    size_t i = L;
    while (i > 0) {
        uint64_t cur;
        i--;
        cur = (rem << 32) | a[i];
        a[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static void fmul(uint32_t *r, const uint32_t *a, const uint32_t *b,
                 size_t L, size_t F, uint32_t *tmp) {
    size_t i, j;
    memset(tmp, 0, 2 * L * sizeof(uint32_t));
    for (i = 0; i < L; i++) {
        uint64_t carry = 0, ai = a[i];
        if (ai == 0) continue;
        for (j = 0; j < L; j++) {
            uint64_t cur = ai * b[j] + tmp[i + j] + carry;
            tmp[i + j] = (uint32_t)cur;
            carry = cur >> 32;
        }
        tmp[i + L] = (uint32_t)carry;
    }
    memcpy(r, tmp + F, L * sizeof(uint32_t));
}

static int iszero(const uint32_t *a, size_t L) {
    size_t i;
    for (i = 0; i < L; i++)
        if (a[i]) return 0;
    return 1;
}

static void mulp_shift(uint32_t *t, const uint32_t *p, size_t m, size_t h,
                       size_t L, uint32_t *tmp) {
    size_t i, j;
    memset(tmp, 0, (L + m) * sizeof(uint32_t));
    for (i = 0; i < m; i++) {
        uint64_t carry = 0, pi = p[i];
        if (pi == 0) continue;
        for (j = 0; j < L; j++) {
            uint64_t cur = pi * t[j] + tmp[i + j] + carry;
            tmp[i + j] = (uint32_t)cur;
            carry = cur >> 32;
        }
        tmp[i + L] = (uint32_t)carry;
    }
    for (i = 0; i < L; i++)
        t[i] = (i + h < L + m) ? tmp[i + h] : 0;
}

static void subf(uint32_t *a, const uint32_t *b, size_t L) {
    uint64_t borrow = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t sub = (uint64_t)b[i] + borrow;
        if ((uint64_t)a[i] >= sub) {
            a[i] = (uint32_t)((uint64_t)a[i] - sub);
            borrow = 0;
        } else {
            a[i] = (uint32_t)(((uint64_t)1 << 32) + a[i] - sub);
            borrow = 1;
        }
    }
}

static unsigned char *digs = NULL;
static size_t ndig = 0;

static void addf(uint32_t *a, const uint32_t *b, size_t L) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t cur = (uint64_t)a[i] + b[i] + carry;
        a[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
}

static void compute(size_t D) {
    size_t bits = D * 3322 / 1000 + 160;
    size_t F = bits / 32 + 2;
    size_t L = F + 1;
    size_t sz = L * sizeof(uint32_t);
    uint32_t *y = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *u = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *ln = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *x = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *S = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *E = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(2 * L + 2, sizeof(uint32_t));
    size_t prec, it, k, a0, b0, i;

    /* 1/sqrt(2) by Newton: y <- y*(3 - 2y^2)/2 */
    y[F - 1] = 0xB504F333u;
    y[F - 2] = 0xF9DE6484u;
    prec = 60;
    it = 2;
    while (prec < bits + 64) { prec *= 2; it++; }
    while (it--) {
        fmul(t, y, y, L, F, tmp);
        addf(t, t, L);
        memset(u, 0, sz);
        u[F] = 3;
        subf(u, t, L);
        fmul(y, y, u, L, F, tmp);
        divs(y, L, 2);
    }
    addf(y, y, L);   /* sqrt(2) */
    y[F] -= 1;       /* sqrt(2) - 1 */

    /* log(2) = 2 * sum 1/((2k+1) 3^(2k+1)) */
    memset(t, 0, sz);
    t[F] = 1;
    divs(t, L, 3);
    memset(ln, 0, sz);
    for (k = 0; !iszero(t, L); k++) {
        memcpy(u, t, sz);
        divs(u, L, (uint32_t)(2 * k + 1));
        addf(ln, u, L);
        divs(t, L, 9);
    }
    addf(ln, ln, L);

    fmul(x, y, ln, L, F, tmp);   /* x = (sqrt(2)-1)*log(2) */

    /* exp(x) as product of exp over limb chunks at depths [a0,b0] */
    memset(E, 0, sz);
    E[F] = 1;
    a0 = 1;
    b0 = 1;
    while (a0 <= F) {
        size_t h = b0 > F ? F : b0;
        size_t m = h - a0 + 1;
        const uint32_t *p = x + (F - h);
        if (!iszero(p, m)) {
            memset(S, 0, sz);
            S[F] = 1;
            memset(t, 0, sz);
            t[F] = 1;
            for (k = 1;; k++) {
                mulp_shift(t, p, m, h, L, tmp);
                if (iszero(t, L)) break;
                divs(t, L, (uint32_t)k);
                if (iszero(t, L)) break;
                addf(S, t, L);
            }
            fmul(E, E, S, L, F, tmp);
        }
        a0 = b0 + 1;
        b0 = 2 * b0;
    }
    addf(E, E, L);   /* 2^sqrt(2) */

    free(digs);
    digs = (unsigned char *)malloc(D + 16);
    digs[0] = (unsigned char)E[F];
    E[F] = 0;
    i = 1;
    while (i < D) {
        uint64_t carry = 0;
        uint32_t chunk;
        size_t j;
        int q;
        for (j = 0; j < F; j++) {
            uint64_t cur = (uint64_t)E[j] * 1000000000u + carry;
            E[j] = (uint32_t)cur;
            carry = cur >> 32;
        }
        chunk = (uint32_t)carry;
        for (q = 8; q >= 0; q--) {
            if (i + (size_t)q < D + 16) digs[i + q] = (unsigned char)(chunk % 10);
            chunk /= 10;
        }
        i += 9;
    }
    ndig = D;

    free(y); free(t); free(u); free(ln); free(x);
    free(S); free(E); free(tmp);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n > ndig) {
        size_t D = (size_t)n + 32;
        if (D < 2 * ndig) D = 2 * ndig;
        if (D < 256) D = 256;
        compute(D);
    }
    return (seq_t)digs[n - 1];
}
