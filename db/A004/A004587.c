/*
 * A004587  Expansion of sqrt(10) in base 4.
 * https://oeis.org/A004587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(10) * 4^(n-1)) mod 4 =
 *            floor(sqrt(10*16^(n-1))) mod 4; computed by the digit-by-digit
 *            square root algorithm in base 4 (radicand 10 = "22"_4)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9998 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int a4587_cmp(const uint32_t *x, const uint32_t *y, size_t L) {
    size_t j = L;
    while (j > 0) {
        j--;
        if (x[j] != y[j]) return x[j] < y[j] ? -1 : 1;
    }
    return 0;
}

/* a = a*16 + add, over L words */
static void a4587_shl4_add(uint32_t *a, size_t L, uint32_t add) {
    uint64_t carry = add;
    size_t j;
    for (j = 0; j < L; j++) {
        uint64_t v = ((uint64_t)a[j] << 4) + carry;
        a[j] = (uint32_t)v;
        carry = v >> 32;
    }
}

/* x -= y, over L words (x >= y) */
static void a4587_sub(uint32_t *x, const uint32_t *y, size_t L) {
    uint64_t borrow = 0;
    size_t j;
    for (j = 0; j < L; j++) {
        uint64_t s = (uint64_t)y[j] + borrow;
        uint64_t xv = x[j];
        if (xv >= s) { x[j] = (uint32_t)(xv - s); borrow = 0; }
        else { x[j] = (uint32_t)(xv + ((uint64_t)1 << 32) - s); borrow = 1; }
    }
}

/* y = d*(8p + d), over L words */
static void a4587_trial(uint32_t *y, const uint32_t *p, size_t L, uint32_t d) {
    uint64_t carry = d;
    size_t j;
    for (j = 0; j < L; j++) {
        uint64_t v = ((uint64_t)p[j] << 3) + carry;
        y[j] = (uint32_t)v;
        carry = v >> 32;
    }
    carry = 0;
    for (j = 0; j < L; j++) {
        uint64_t v = (uint64_t)y[j] * d + carry;
        y[j] = (uint32_t)v;
        carry = v >> 32;
    }
}

static unsigned char *a4587_dig = NULL;
static idx_t a4587_ndig = 0;

static void a4587_compute(idx_t N) {
    size_t cap = (size_t)(N / 16) + 8;
    uint32_t *p = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *y = (uint32_t *)calloc(cap, sizeof(uint32_t));
    unsigned char *dig = (unsigned char *)malloc((size_t)N);
    idx_t i;
    for (i = 0; i < N; i++) {
        size_t L = (size_t)((2 * i + 8) / 32) + 2;
        uint32_t d;
        if (L > cap) L = cap;
        a4587_shl4_add(r, L, i == 0 ? 10u : 0u);
        for (d = 3; d > 0; d--) {
            a4587_trial(y, p, L, d);
            if (a4587_cmp(y, r, L) <= 0) break;
        }
        if (d > 0) a4587_sub(r, y, L);
        /* p = 4p + d */
        {
            uint64_t carry = d;
            size_t j;
            for (j = 0; j < L; j++) {
                uint64_t v = ((uint64_t)p[j] << 2) + carry;
                p[j] = (uint32_t)v;
                carry = v >> 32;
            }
        }
        dig[i] = (unsigned char)d;
    }
    free(p);
    free(r);
    free(y);
    free(a4587_dig);
    a4587_dig = dig;
    a4587_ndig = N;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n > a4587_ndig) {
        idx_t target = n;
        if (target < 2 * a4587_ndig) target = 2 * a4587_ndig;
        if (target < 64) target = 64;
        a4587_compute(target);
    }
    return (seq_t)a4587_dig[n - 1];
}
