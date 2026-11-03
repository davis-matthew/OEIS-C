/*
 * A007514  Pi = Sum_{n >= 0} a(n)/n!.
 * https://oeis.org/A007514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n!*Pi) - n*floor((n-1)!*Pi), a(0) = 3; computed as
 *            the factorial-base digits of Pi: with f = frac(Pi), a(n) =
 *            floor(n*f_{n-1}), f_n = n*f_{n-1} - a(n), using exact
 *            fixed-point Pi from Machin's formula Pi = 16*atan(1/5) -
 *            4*atan(1/239) at about log2(n!) + guard bits
 * Range:     u8 n <= 268, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 7899 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void a007514_add(uint32_t *s, const uint32_t *q, size_t st, size_t len) {
    uint64_t carry = 0;
    size_t i = len;
    while (i > st) {
        i--;
        uint64_t cur = (uint64_t)s[i] + q[i] + carry;
        s[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
    while (carry && i > 0) {
        i--;
        uint64_t cur = (uint64_t)s[i] + carry;
        s[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
}

static void a007514_div25(uint32_t *a, size_t from, size_t len) {
    uint64_t rem = 0;
    size_t i;
    for (i = from; i < len; i++) {
        uint64_t cur = (rem << 32) | a[i];
        uint64_t qq = cur / 25u;
        a[i] = (uint32_t)qq;
        rem = cur - qq * 25u;
    }
}

static void a007514_div57121(uint32_t *a, size_t from, size_t len) {
    uint64_t rem = 0;
    size_t i;
    for (i = from; i < len; i++) {
        uint64_t cur = (rem << 32) | a[i];
        uint64_t qq = cur / 57121u;
        a[i] = (uint32_t)qq;
        rem = cur - qq * 57121u;
    }
}

static void a007514_divsmall(uint32_t *a, size_t from, size_t len, uint32_t d) {
    uint64_t rem = 0;
    size_t i;
    for (i = from; i < len; i++) {
        uint64_t cur = (rem << 32) | a[i];
        a[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static void a007514_sub(uint32_t *s, const uint32_t *q, size_t st, size_t len) {
    int64_t borrow = 0;
    size_t i = len;
    while (i > st) {
        i--;
        int64_t cur = (int64_t)s[i] - (int64_t)q[i] - borrow;
        if (cur < 0) { cur += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        s[i] = (uint32_t)cur;
    }
    while (borrow && i > 0) {
        i--;
        int64_t cur = (int64_t)s[i] - borrow;
        if (cur < 0) { cur += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        s[i] = (uint32_t)cur;
    }
}

/* s = atan(1/x) in fixed point: limb 0 is the integer part */
static void a007514_atan_inv(uint32_t x, uint32_t *s, uint32_t *t, uint32_t *q, size_t len) {
    size_t st = 0;
    uint32_t k;
    memset(t, 0, len * sizeof(uint32_t));
    t[0] = 1;
    a007514_divsmall(t, 0, len, x);
    memcpy(s, t, len * sizeof(uint32_t));
    for (k = 1;; k++) {
        if (x == 5) a007514_div25(t, st, len);
        else a007514_div57121(t, st, len);
        while (st < len && t[st] == 0) st++;
        if (st == len) break;
        memcpy(q + st, t + st, (len - st) * sizeof(uint32_t));
        a007514_divsmall(q, st, len, 2 * k + 1);
        if (k & 1) a007514_sub(s, q, st, len);
        else a007514_add(s, q, st, len);
    }
}

static void a007514_mulsmall(uint32_t *a, size_t len, uint32_t m) {
    uint64_t carry = 0;
    size_t i = len;
    while (i > 0) {
        i--;
        uint64_t cur = (uint64_t)a[i] * m + carry;
        a[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
}

static uint32_t *a007514_dig = 0;
static idx_t a007514_cap = 0;

static void a007514_compute(idx_t N) {
    uint64_t bits = 128;
    idx_t k;
    for (k = 2; k <= N; k++) {
        idx_t v = k;
        while (v) { bits++; v >>= 1; }
    }
    size_t len = (size_t)(1 + bits / 32 + 3);
    uint32_t *s5 = (uint32_t *)malloc(len * sizeof(uint32_t));
    uint32_t *s239 = (uint32_t *)malloc(len * sizeof(uint32_t));
    uint32_t *t = (uint32_t *)malloc(len * sizeof(uint32_t));
    uint32_t *q = (uint32_t *)malloc(len * sizeof(uint32_t));
    a007514_atan_inv(5, s5, t, q, len);
    a007514_atan_inv(239, s239, t, q, len);
    a007514_mulsmall(s5, len, 16);
    a007514_mulsmall(s239, len, 4);
    a007514_sub(s5, s239, 0, len);  /* s5 = Pi */

    uint32_t *dig = (uint32_t *)malloc((size_t)(N + 1) * sizeof(uint32_t));
    dig[0] = s5[0];
    s5[0] = 0;
    for (k = 1; k <= N; k++) {
        a007514_mulsmall(s5, len, (uint32_t)k);
        dig[k] = s5[0];
        s5[0] = 0;
    }
    free(s5); free(s239); free(t); free(q);
    free(a007514_dig);
    a007514_dig = dig;
    a007514_cap = N + 1;
}

seq_t a(idx_t n) {
    if (n >= a007514_cap) {
        idx_t N = n;
        if (N < a007514_cap + a007514_cap / 4) N = a007514_cap + a007514_cap / 4;
        if (N < 32) N = 32;
        a007514_compute(N);
    }
    return (seq_t)a007514_dig[n];
}
