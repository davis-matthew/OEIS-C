/*
 * A002701  Coefficients for numerical differentiation.
 * https://oeis.org/A002701
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator(6 * Sum_{j=1..k} 1/j^2 /
 *            ((2k+1)*binomial(2k,k))) with k = n-1, i.e. a(n) = numer(6 *
 *            A002455(n) / 2^(2n) / (2n+3)!) -- the coefficient of
 *            mu*delta^(2n-1) in the central-difference expansion of the third
 *            derivative (1, 1/4, 7/120, 41/3024, ...)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 23 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void x2701_add(uint32_t *x, size_t *xl, const uint32_t *y, size_t yl) {
    uint64_t c = 0;
    size_t L = (*xl > yl) ? *xl : yl;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)x[i] + (i < yl ? y[i] : 0) + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) x[L++] = (uint32_t)c;
    *xl = L;
}

static void x2701_div(const uint32_t *x, size_t len, uint32_t d, uint32_t *q, size_t *qlen) {
    uint64_t r = 0;
    size_t i = len;
    while (i > 0) {
        i--;
        uint64_t cur = (r << 32) | x[i];
        q[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
    *qlen = len;
    while (*qlen > 0 && q[*qlen - 1] == 0) (*qlen)--;
}

static uint32_t x2701_mod(const uint32_t *x, size_t len, uint32_t d) {
    uint64_t r = 0;
    size_t i = len;
    while (i > 0) {
        i--;
        r = ((r << 32) | x[i]) % d;
    }
    return (uint32_t)r;
}

static void x2701_mul(uint32_t *x, size_t *len, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < *len; i++) {
        uint64_t t = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
    while (c) {
        x[(*len)++] = (uint32_t)c;
        c >>= 32;
    }
}

seq_t a(idx_t n) {
    idx_t k = n - 1;
    idx_t M = 2 * k + 1;
    idx_t i, j, p;
    uint32_t *spf = (uint32_t *)calloc((size_t)M + 1, sizeof(uint32_t));
    for (i = 2; i <= M; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= M; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }

    size_t cap = (size_t)(k / 8) + 16;
    uint32_t *D = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *N = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)calloc(cap, sizeof(uint32_t));
    size_t dl = 1, nl = 0, tl = 0;
    D[0] = 1;

    /* N/D = Sum_{j<=k} 1/j^2 with D = lcm(1..k)^2 */
    for (j = 1; j <= k; j++) {
        if (j > 1) {
            idx_t q = spf[j], m = j;
            while (m % q == 0) m /= q;
            if (m == 1) {
                x2701_mul(D, &dl, (uint32_t)(q * q));
                x2701_mul(N, &nl, (uint32_t)(q * q));
            }
        }
        x2701_div(D, dl, (uint32_t)(j * j), T, &tl);
        x2701_add(N, &nl, T, tl);
    }
    x2701_mul(N, &nl, 6);

    /* cancel against denominator lcm^2 * (2k+1) * C(2k,k) */
    for (p = 2; p <= M; p++) {
        if (spf[p] != p) continue;
        long long e = 0;
        uint64_t pp;
        for (pp = p; pp <= k; pp *= p) e += 2;
        idx_t t = M;
        while (t % p == 0) { t /= p; e++; }
        for (pp = p; pp <= 2 * k; pp *= p)
            e += (long long)((2 * k) / pp - 2 * (k / pp));
        while (e > 0 && nl > 0 && x2701_mod(N, nl, (uint32_t)p) == 0) {
            x2701_div(N, nl, (uint32_t)p, N, &nl);
            e--;
        }
    }

    seq_t res = 0;
    size_t idx = nl;
    while (idx > 0) {
        idx--;
        int b;
        for (b = 3; b >= 0; b--) {
            res = (seq_t)(res * 256);
            res += (seq_t)((N[idx] >> (8 * b)) & 0xFFu);
        }
    }

    free(spf);
    free(D);
    free(N);
    free(T);
    return res;
}
