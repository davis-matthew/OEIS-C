/*
 * A001622  Decimal expansion of golden ratio phi (or tau) = (1 + sqrt(5))/2.
 * https://oeis.org/A001622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, core, nice, easy, changed
 * Formula:   digits of floor(phi*10^m) = floor((10^m +
 *            floor(sqrt(5*10^(2m))))/2); sqrt(5) computed digit by digit
 *            (exact integer square root), a(n) = n-th digit
 * Range:     u8 n <= 9003, u16 n <= 9003, u32 n <= 9003, u64 n <= 9003,
 *            u128 n <= 9003, big any n
 * Helpers:   phi_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/phi_cmp.h"

#define PHI_BASE 1000000000u

static void phi_mul_small(uint32_t *x, size_t L, uint32_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t v = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)(v % PHI_BASE);
        carry = v / PHI_BASE;
    }
}

static void phi_add_small(uint32_t *x, size_t L, uint32_t v) {
    uint64_t carry = v;
    for (size_t i = 0; i < L && carry; i++) {
        uint64_t s = (uint64_t)x[i] + carry;
        x[i] = (uint32_t)(s % PHI_BASE);
        carry = s / PHI_BASE;
    }
}

static void phi_sub(uint32_t *a, const uint32_t *b, size_t L) {
    int64_t borrow = 0;
    for (size_t i = 0; i < L; i++) {
        int64_t v = (int64_t)a[i] - (int64_t)b[i] - borrow;
        if (v < 0) { v += PHI_BASE; borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)v;
    }
}

static uint8_t *phi_digits = NULL;
static size_t phi_have = 0;

static void phi_compute(size_t M) {
    size_t L = M / 9 + 4;
    uint32_t *p = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *d = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint8_t *s = (uint8_t *)malloc(M);
    uint8_t *out = (uint8_t *)malloc(M);

    for (size_t i = 0; i < M; i++) {
        phi_mul_small(r, L, 100);
        phi_add_small(r, L, i == 0 ? 5 : 0);
        memcpy(d, p, L * sizeof(uint32_t));
        phi_mul_small(d, L, 20);
        uint32_t lo = 0, hi = 9;
        while (lo < hi) {
            uint32_t mid = (lo + hi + 1) / 2;
            memcpy(t, d, L * sizeof(uint32_t));
            phi_add_small(t, L, mid);
            phi_mul_small(t, L, mid);
            if (phi_cmp(t, r, L) <= 0) lo = mid; else hi = mid - 1;
        }
        memcpy(t, d, L * sizeof(uint32_t));
        phi_add_small(t, L, lo);
        phi_mul_small(t, L, lo);
        phi_sub(r, t, L);
        phi_mul_small(p, L, 10);
        phi_add_small(p, L, lo);
        s[i] = (uint8_t)lo;
    }

    /* s = 2236...; add 10^(M-1) (leading digit 2 -> 3), then halve */
    s[0] += 1;
    unsigned rem = 0;
    for (size_t i = 0; i < M; i++) {
        unsigned v = rem * 10 + s[i];
        out[i] = (uint8_t)(v / 2);
        rem = v % 2;
    }

    free(p); free(r); free(d); free(t); free(s);
    free(phi_digits);
    phi_digits = out;
    phi_have = M;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if ((size_t)n + 5 > phi_have) {
        size_t M = phi_have * 2;
        if (M < (size_t)n + 20) M = (size_t)n + 20;
        if (M < 64) M = 64;
        phi_compute(M);
    }
    return (seq_t)phi_digits[n - 1];
}
