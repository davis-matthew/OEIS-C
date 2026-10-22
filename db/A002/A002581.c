/*
 * A002581  Decimal expansion of cube root of 3.
 * https://oeis.org/A002581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(3^(1/3) * 10^(n-1)) mod 10; 3^(-1/3) by Newton y <-
 *            y + y*(1-3y^3)/3 in base-10^9 fixed point, then 3^(1/3) = 3*y^2
 * Range:     u8 n <= 9764, u16 n <= 9764, u32 n <= 9764, u64 n <= 9764,
 *            u128 n <= 9764, big any n
 * Helpers:   c3r_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/c3r_cmp.h"

#define C3R_B 1000000000ULL

static void c3r_mul(const uint32_t *x, const uint32_t *y, uint32_t *r,
                    size_t L, size_t F, uint64_t *tmp) {
    size_t i, j;
    memset(tmp, 0, 2 * L * sizeof(uint64_t));
    for (i = 0; i < L; i++) {
        uint64_t carry = 0, xi = x[i];
        if (xi == 0) continue;
        for (j = 0; j < L; j++) {
            uint64_t cur = tmp[i + j] + xi * (uint64_t)y[j] + carry;
            tmp[i + j] = cur % C3R_B;
            carry = cur / C3R_B;
        }
        tmp[i + L] += carry;
    }
    for (i = 0; i < L; i++) r[i] = (uint32_t)tmp[i + F];
}

static void c3r_muls(uint32_t *x, size_t L, uint32_t m) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t cur = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)(cur % C3R_B);
        carry = cur / C3R_B;
    }
}

static void c3r_divs(uint32_t *x, size_t L, uint32_t d) {
    uint64_t rem = 0;
    size_t i = L;
    while (i-- > 0) {
        uint64_t cur = rem * C3R_B + x[i];
        x[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

/* r = x - y, requires x >= y */
static void c3r_sub(const uint32_t *x, const uint32_t *y, uint32_t *r, size_t L) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        int64_t cur = (int64_t)x[i] - (int64_t)y[i] - borrow;
        if (cur < 0) { cur += (int64_t)C3R_B; borrow = 1; } else borrow = 0;
        r[i] = (uint32_t)cur;
    }
}

static void c3r_add(uint32_t *x, const uint32_t *y, size_t L) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t cur = (uint64_t)x[i] + y[i] + carry;
        x[i] = (uint32_t)(cur % C3R_B);
        carry = cur / C3R_B;
    }
}

static uint32_t *c3r_cache = NULL;
static size_t c3r_F = 0;

static void c3r_compute(size_t F) {
    size_t L = F + 1;
    uint32_t *y = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t1 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t2 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *one = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint64_t *tmp = (uint64_t *)malloc(2 * L * sizeof(uint64_t));
    size_t acc = 8, iters = 0, it;

    one[F] = 1;
    y[F - 1] = 693361274u; /* 3^(-1/3) ~ 0.693361274 */
    while (acc < 9 * F + 18) { acc *= 2; iters++; }
    iters += 3;

    for (it = 0; it < iters; it++) {
        c3r_mul(y, y, t1, L, F, tmp);
        c3r_mul(t1, y, t2, L, F, tmp);
        c3r_muls(t2, L, 3);              /* t2 = 3*y^3 */
        if (c3r_cmp(t2, one, L) <= 0) {
            c3r_sub(one, t2, t1, L);     /* t1 = 1 - 3y^3 */
            c3r_mul(y, t1, t2, L, F, tmp);
            c3r_divs(t2, L, 3);
            c3r_add(y, t2, L);
        } else {
            c3r_sub(t2, one, t1, L);     /* t1 = 3y^3 - 1 */
            c3r_mul(y, t1, t2, L, F, tmp);
            c3r_divs(t2, L, 3);
            c3r_sub(y, t2, y, L);
        }
    }
    c3r_mul(y, y, t1, L, F, tmp);
    c3r_muls(t1, L, 3);                  /* t1 = 3*y^2 = 3^(1/3) */

    free(c3r_cache);
    c3r_cache = t1;
    c3r_F = F;
    free(y); free(t2); free(one); free(tmp);
}

seq_t a(idx_t n) {
    idx_t d;
    size_t limb, pos, k;
    uint32_t v;
    if (n < 1) return (seq_t)0;
    if (c3r_cache == NULL || (n - 1) + 30 > 9 * (idx_t)c3r_F) {
        size_t F = (size_t)((n + 40) / 9 + 2);
        if (F < 2 * c3r_F) F = 2 * c3r_F;
        if (F < 8) F = 8;
        c3r_compute(F);
    }
    if (n == 1) return (seq_t)(c3r_cache[c3r_F] % 10);
    d = n - 1; /* d-th digit after the decimal point, d >= 1 */
    limb = c3r_F - 1 - (size_t)((d - 1) / 9);
    pos = (size_t)((d - 1) % 9);
    v = c3r_cache[limb];
    for (k = 0; k < 8 - pos; k++) v /= 10;
    return (seq_t)(v % 10);
}
