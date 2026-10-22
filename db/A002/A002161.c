/*
 * A002161  Decimal expansion of square root of Pi.
 * https://oeis.org/A002161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   digits of sqrt(Pi) = Pi * (1/sqrt(Pi)); Pi from Machin's formula
 *            Pi = 16*arctan(1/5) - 4*arctan(1/239); 1/sqrt(Pi) by Newton
 *            iteration y <- y*(3 - Pi*y^2)/2, in base-10^9 fixed-point
 *            arithmetic; a(n) = n-th digit
 * Range:     u8 n <= 8381, u16 n <= 8381, u32 n <= 8381, u64 n <= 8381,
 *            u128 n <= 8381, big any n
 * Helpers:   q_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/q_zero.h"

#define Q_BASE 1000000000ULL

static char *q_digits = NULL;
static idx_t q_count = 0;

/* little-endian limbs, x[F] is the integer limb, L = F + 1 */
static void q_divs(uint32_t *x, size_t L, uint64_t d) {
    uint64_t r = 0;
    size_t i = L;
    while (i-- > 0) {
        uint64_t cur = r * Q_BASE + x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static void q_muls(uint32_t *x, size_t L, uint64_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)(t % Q_BASE);
        c = t / Q_BASE;
    }
}

static void q_add(uint32_t *a, const uint32_t *b, size_t L) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)a[i] + b[i] + c;
        a[i] = (uint32_t)(t % Q_BASE);
        c = t / Q_BASE;
    }
}

static void q_sub(uint32_t *a, const uint32_t *b, size_t L) {
    int64_t br = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        int64_t t = (int64_t)a[i] - (int64_t)b[i] - br;
        if (t < 0) { t += (int64_t)Q_BASE; br = 1; } else br = 0;
        a[i] = (uint32_t)t;
    }
}

static void q_mul(const uint32_t *a, const uint32_t *b, uint32_t *out,
                  uint32_t *prod, size_t L, size_t F) {
    size_t i, j;
    memset(prod, 0, 2 * L * sizeof(uint32_t));
    for (i = 0; i < L; i++) {
        uint64_t c = 0;
        if (a[i] == 0) continue;
        for (j = 0; j < L; j++) {
            uint64_t t = (uint64_t)prod[i + j] + (uint64_t)a[i] * b[j] + c;
            prod[i + j] = (uint32_t)(t % Q_BASE);
            c = t / Q_BASE;
        }
        prod[i + L] = (uint32_t)c;
    }
    for (i = 0; i < L; i++) out[i] = prod[F + i];
}

static void q_atan(uint32_t *sum, size_t L, size_t F, uint64_t x,
                   uint32_t *p, uint32_t *t) {
    uint64_t k = 1;
    memset(p, 0, L * sizeof(uint32_t));
    p[F] = 1;
    q_divs(p, L, x);
    memcpy(sum, p, L * sizeof(uint32_t));
    for (;;) {
        q_divs(p, L, x * x);
        if (q_zero(p, L)) break;
        memcpy(t, p, L * sizeof(uint32_t));
        q_divs(t, L, 2 * k + 1);
        if (k & 1) q_sub(sum, t, L); else q_add(sum, t, L);
        k++;
    }
}

static void q_compute(size_t F) {
    size_t L = F + 1, i;
    uint32_t *pi = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *a2 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *p = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *u = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *y = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *prod = (uint32_t *)calloc(2 * L, sizeof(uint32_t));
    uint64_t prec;
    int extra;
    idx_t cnt;

    q_atan(pi, L, F, 5, p, t);
    q_muls(pi, L, 16);
    q_atan(a2, L, F, 239, p, t);
    q_muls(a2, L, 4);
    q_sub(pi, a2, L);

    /* 1/sqrt(Pi) = 0.564189583547756286948... */
    y[F - 1] = 564189583u;
    if (F >= 2) y[F - 2] = 547756286u;
    prec = 16;
    extra = 2;
    while (prec < 9 * (uint64_t)F + 18 || extra-- > 0) {
        q_mul(y, y, t, prod, L, F);
        q_mul(pi, t, u, prod, L, F);
        memset(a2, 0, L * sizeof(uint32_t));
        a2[F] = 3;
        q_sub(a2, u, L);
        q_mul(y, a2, t, prod, L, F);
        q_divs(t, L, 2);
        memcpy(y, t, L * sizeof(uint32_t));
        if (prec < 9 * (uint64_t)F + 18) prec *= 2;
    }
    q_mul(pi, y, t, prod, L, F);

    cnt = 1 + 9 * (idx_t)(F - 4);
    free(q_digits);
    q_digits = (char *)malloc((size_t)cnt + 1);
    q_digits[0] = (char)(t[F] % 10);
    for (i = 1; i < (size_t)cnt; i++) {
        size_t k = i - 1;
        uint32_t limb = t[F - 1 - k / 9];
        int pos = (int)(k % 9), e;
        for (e = 0; e < 8 - pos; e++) limb /= 10;
        q_digits[i] = (char)(limb % 10);
    }
    q_count = cnt;

    free(pi); free(a2); free(p); free(t); free(u); free(y); free(prod);
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > q_count) {
        idx_t target = n < 1000 ? 1000 : n;
        q_compute((size_t)(target / 9 + 8));
    }
    return (seq_t)(unsigned)q_digits[n - 1];
}
