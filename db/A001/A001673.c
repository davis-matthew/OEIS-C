/*
 * A001673  a(n) = ceiling(Pi^n).
 * https://oeis.org/A001673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(Pi^n); Pi from Machin's formula Pi =
 *            16*arctan(1/5) - 4*arctan(1/239) in fixed point, then Pi^n by
 *            binary powering with all roundings downward, so a(n) =
 *            floor(computed Pi^n) + 1 for n >= 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 285 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void p1673_add(uint32_t *a, const uint32_t *b, size_t L) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)a[i] + b[i] + c;
        a[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void p1673_divs(uint32_t *x, size_t L, uint32_t d) {
    uint64_t r = 0;
    size_t i;
    for (i = L; i > 0; i--) {
        r = (r << 32) | x[i - 1];
        x[i - 1] = (uint32_t)(r / d);
        r %= d;
    }
}

static void p1673_sub(uint32_t *a, const uint32_t *b, size_t L) {
    int64_t br = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        int64_t t = (int64_t)a[i] - (int64_t)b[i] - br;
        br = t < 0 ? 1 : 0;
        if (t < 0) t += (int64_t)1 << 32;
        a[i] = (uint32_t)t;
    }
}

static int p1673_zero(const uint32_t *x, size_t L) {
    size_t i;
    for (i = 0; i < L; i++)
        if (x[i]) return 0;
    return 1;
}

/* sum = arctan(1/x) with M-1 fractional limbs and 1 integer limb */
static void p1673_atan(uint32_t *sum, size_t M, uint32_t x) {
    uint32_t *t = (uint32_t *)calloc(M, sizeof(uint32_t));
    uint32_t *term = (uint32_t *)malloc(M * sizeof(uint32_t));
    uint32_t x2 = x * x;
    uint64_t k = 1;
    t[M - 1] = 1;
    p1673_divs(t, M, x);
    memcpy(sum, t, M * sizeof(uint32_t));
    for (;;) {
        p1673_divs(t, M, x2);
        if (p1673_zero(t, M)) break;
        memcpy(term, t, M * sizeof(uint32_t));
        p1673_divs(term, M, (uint32_t)(2 * k + 1));
        if (k & 1) p1673_sub(sum, term, M);
        else p1673_add(sum, term, M);
        k++;
    }
    free(t);
    free(term);
}

/* returns floor(a*b / 2^(32*FL)), trimmed */
static uint32_t *p1673_mulfx(const uint32_t *a, size_t la, const uint32_t *b,
                             size_t lb, size_t FL, size_t *lo) {
    size_t n = la + lb, i, j, len;
    uint32_t *tmp = (uint32_t *)calloc(n, sizeof(uint32_t));
    uint32_t *out;
    for (i = 0; i < la; i++) {
        uint64_t c = 0;
        if (!a[i]) continue;
        for (j = 0; j < lb; j++) {
            uint64_t t = (uint64_t)a[i] * b[j] + tmp[i + j] + c;
            tmp[i + j] = (uint32_t)t;
            c = t >> 32;
        }
        tmp[i + lb] = (uint32_t)c;
    }
    len = n - FL;
    while (len > FL + 1 && tmp[FL + len - 1] == 0) len--;
    out = (uint32_t *)malloc(len * sizeof(uint32_t));
    memcpy(out, tmp + FL, len * sizeof(uint32_t));
    free(tmp);
    *lo = len;
    return out;
}

static void p1673_muls(uint32_t *x, size_t L, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}

seq_t a(idx_t n) {
    size_t FL, M, i, lx, lb;
    uint32_t *a5, *a239, *X, *B;
    uint64_t c;
    idx_t e;
    seq_t r;
    int s;

    if (n == 0) return 1;

    FL = (size_t)((17 * n) / 10 + 96) / 32 + 1;
    M = FL + 3; /* 2 guard limbs + 1 integer limb */

    a5 = (uint32_t *)malloc(M * sizeof(uint32_t));
    a239 = (uint32_t *)malloc(M * sizeof(uint32_t));
    p1673_atan(a5, M, 5);
    p1673_atan(a239, M, 239);
    p1673_muls(a5, M, 16);
    p1673_muls(a239, M, 4);
    p1673_sub(a5, a239, M);
    /* subtract a margin exceeding the series error so Pi is underestimated */
    memset(a239, 0, M * sizeof(uint32_t));
    a239[0] = (uint32_t)1 << 20;
    p1673_sub(a5, a239, M);
    free(a239);

    lb = FL + 1;
    B = (uint32_t *)malloc(lb * sizeof(uint32_t));
    memcpy(B, a5 + 2, lb * sizeof(uint32_t));
    free(a5);

    lx = FL + 1;
    X = (uint32_t *)calloc(lx, sizeof(uint32_t));
    X[FL] = 1;

    e = n;
    while (e) {
        if (e & 1) {
            size_t nl;
            uint32_t *t = p1673_mulfx(X, lx, B, lb, FL, &nl);
            free(X);
            X = t;
            lx = nl;
        }
        e >>= 1;
        if (e) {
            size_t nl;
            uint32_t *t = p1673_mulfx(B, lb, B, lb, FL, &nl);
            free(B);
            B = t;
            lb = nl;
        }
    }
    free(B);

    r = 0;
    c = 0;
    for (i = lx; i > FL; i--) {
        uint32_t w = X[i - 1];
        if (c == 0 && w == 0) continue;
        c = 1;
        for (s = 24; s >= 0; s -= 8) {
            r <<= 8;
            r |= (seq_t)((w >> s) & 255u);
        }
    }
    free(X);
    return r + 1;
}
