/*
 * A002459  Nearest integer to cosh(n).
 * https://oeis.org/A002459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = round(cosh(n)) = floor((e^n + e^(-n))/2 + 1/2), with e
 *            and 1/e computed exactly enough in fixed-point big arithmetic
 *            (sum of 1/k!, then binary powering)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 130 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void c2459_add(uint32_t *a, const uint32_t *b, size_t L) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)a[i] + b[i] + c;
        a[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void c2459_divs(uint32_t *x, size_t L, uint32_t d) {
    uint64_t r = 0;
    size_t i = L;
    while (i-- > 0) {
        uint64_t cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static int c2459_iszero(const uint32_t *x, size_t L) {
    size_t i;
    for (i = 0; i < L; i++)
        if (x[i]) return 0;
    return 1;
}

static size_t c2459_len(const uint32_t *x, size_t L) {
    while (L > 0 && x[L - 1] == 0) L--;
    return L;
}

/* out = (a*b) >> (32*FL); tmp has room for 2L limbs */
static void c2459_mulfx(uint32_t *out, const uint32_t *a, const uint32_t *b,
                        size_t L, size_t FL, uint32_t *tmp) {
    size_t la = c2459_len(a, L), lb = c2459_len(b, L);
    size_t i, j, k;
    memset(tmp, 0, sizeof(uint32_t) * 2 * L);
    for (i = 0; i < la; i++) {
        uint64_t carry = 0;
        uint64_t ai = a[i];
        for (j = 0; j < lb; j++) {
            uint64_t t = ai * b[j] + tmp[i + j] + carry;
            tmp[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        tmp[i + lb] = (uint32_t)carry;
    }
    for (k = 0; k < L; k++)
        out[k] = (k + FL < 2 * L) ? tmp[k + FL] : 0;
}

static void c2459_pow(uint32_t *R, const uint32_t *base, idx_t e,
                      size_t L, size_t FL, uint32_t *B, uint32_t *tmp) {
    memset(R, 0, sizeof(uint32_t) * L);
    R[FL] = 1;
    memcpy(B, base, sizeof(uint32_t) * L);
    while (e) {
        if (e & 1) c2459_mulfx(R, R, B, L, FL, tmp);
        e >>= 1;
        if (e) c2459_mulfx(B, B, B, L, FL, tmp);
    }
}

static void c2459_sub(uint32_t *a, const uint32_t *b, size_t L) {
    uint64_t br = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)a[i] - b[i] - br;
        a[i] = (uint32_t)t;
        br = (t >> 63) & 1;
    }
}

seq_t a(idx_t n) {
    uint64_t ib = (uint64_t)n * 1443 / 1000 + 2;
    size_t FL = (size_t)((ib + 96) / 32 + 1);
    size_t IL = (size_t)(ib / 32 + 2);
    size_t L = FL + IL;
    size_t i;
    uint32_t k;
    uint32_t *T = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *P = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *N = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *E = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *Ei = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *R1 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *R2 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *B = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(2 * L, sizeof(uint32_t));
    seq_t r = 0;

    /* e = sum 1/k!, 1/e = sum (-1)^k/k! */
    T[FL] = 1;
    memcpy(P, T, sizeof(uint32_t) * L);
    for (k = 1;; k++) {
        c2459_divs(T, L, k);
        if (c2459_iszero(T, L)) break;
        if (k & 1) c2459_add(N, T, L);
        else c2459_add(P, T, L);
    }
    memcpy(E, P, sizeof(uint32_t) * L);
    c2459_add(E, N, L);
    memcpy(Ei, P, sizeof(uint32_t) * L);
    c2459_sub(Ei, N, L);

    c2459_pow(R1, E, n, L, FL, B, tmp);
    c2459_pow(R2, Ei, n, L, FL, B, tmp);

    /* W = e^n + e^-n + 1, then floor(W / 2) */
    c2459_add(R1, R2, L);
    memset(T, 0, sizeof(uint32_t) * L);
    T[FL] = 1;
    c2459_add(R1, T, L);
    for (i = 0; i < L; i++) {
        uint32_t hi = (i + 1 < L) ? R1[i + 1] : 0;
        R1[i] = (R1[i] >> 1) | (hi << 31);
    }

    i = L;
    while (i-- > FL) {
        uint32_t w = R1[i];
        r = r * 256 + ((w >> 24) & 0xFF);
        r = r * 256 + ((w >> 16) & 0xFF);
        r = r * 256 + ((w >> 8) & 0xFF);
        r = r * 256 + (w & 0xFF);
    }

    free(T); free(P); free(N); free(E); free(Ei);
    free(R1); free(R2); free(B); free(tmp);
    return r;
}
