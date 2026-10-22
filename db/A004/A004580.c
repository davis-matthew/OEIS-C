/*
 * A004580  Expansion of sqrt(8) in base 5.
 * https://oeis.org/A004580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(8) * 5^(n-1)) mod 5; digits computed by the
 *            digit-by-digit (long-hand) square root algorithm in base 5 with
 *            multi-limb integers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a4580_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a4580_cmp.h"

#define A4580_LB 1220703125u /* 5^13 */

static uint32_t *a4580_P = NULL, *a4580_R = NULL, *a4580_Q = NULL, *a4580_T = NULL;
static size_t a4580_pl = 0, a4580_rl = 0, a4580_cap = 0;
static unsigned char *a4580_dig = NULL;
static size_t a4580_nd = 0, a4580_dcap = 0;

static void a4580_ensure(size_t c) {
    if (c <= a4580_cap) return;
    a4580_P = (uint32_t *)realloc(a4580_P, c * sizeof(uint32_t));
    a4580_R = (uint32_t *)realloc(a4580_R, c * sizeof(uint32_t));
    a4580_Q = (uint32_t *)realloc(a4580_Q, c * sizeof(uint32_t));
    a4580_T = (uint32_t *)realloc(a4580_T, c * sizeof(uint32_t));
    a4580_cap = c;
}

static void a4580_mul_small(uint32_t *x, size_t *len, uint32_t m) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < *len; i++) {
        uint64_t v = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)(v % A4580_LB);
        carry = v / A4580_LB;
    }
    while (carry) {
        x[(*len)++] = (uint32_t)(carry % A4580_LB);
        carry /= A4580_LB;
    }
}

static void a4580_add_small(uint32_t *x, size_t *len, uint32_t a) {
    uint64_t carry = a;
    size_t i = 0;
    while (carry) {
        uint64_t v;
        if (i == *len) x[(*len)++] = 0;
        v = (uint64_t)x[i] + carry;
        x[i] = (uint32_t)(v % A4580_LB);
        carry = v / A4580_LB;
        i++;
    }
}

static void a4580_sub(uint32_t *a, size_t *al, const uint32_t *b, size_t bl) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < *al; i++) {
        int64_t v = (int64_t)a[i] - borrow - (i < bl ? (int64_t)b[i] : 0);
        if (v < 0) { v += A4580_LB; borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)v;
    }
    while (*al > 0 && a[*al - 1] == 0) (*al)--;
}

static void a4580_extend(size_t target) {
    if (target > a4580_dcap) {
        size_t nc = target * 2 + 16;
        a4580_dig = (unsigned char *)realloc(a4580_dig, nc);
        a4580_dcap = nc;
    }
    a4580_ensure((target + 8) / 13 + 6);
    if (a4580_nd == 0) {
        /* first pair: 8 -> digit 2, remainder 4 */
        a4580_P[0] = 2; a4580_pl = 1;
        a4580_R[0] = 4; a4580_rl = 1;
        a4580_dig[0] = 2;
        a4580_nd = 1;
    }
    while (a4580_nd < target) {
        size_t ql, tl;
        uint32_t d, chosen = 0;
        /* c = 25 * r */
        a4580_mul_small(a4580_R, &a4580_rl, 25);
        /* q = 10 * p */
        memcpy(a4580_Q, a4580_P, a4580_pl * sizeof(uint32_t));
        ql = a4580_pl;
        a4580_mul_small(a4580_Q, &ql, 10);
        for (d = 4; d >= 1; d--) {
            memcpy(a4580_T, a4580_Q, ql * sizeof(uint32_t));
            tl = ql;
            a4580_mul_small(a4580_T, &tl, d);
            a4580_add_small(a4580_T, &tl, d * d);
            if (a4580_cmp(a4580_T, tl, a4580_R, a4580_rl) <= 0) {
                a4580_sub(a4580_R, &a4580_rl, a4580_T, tl);
                chosen = d;
                break;
            }
        }
        a4580_mul_small(a4580_P, &a4580_pl, 5);
        a4580_add_small(a4580_P, &a4580_pl, chosen);
        a4580_dig[a4580_nd++] = (unsigned char)chosen;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if ((size_t)n > a4580_nd) {
        size_t target = (size_t)n;
        if (target < 2 * a4580_nd) target = 2 * a4580_nd;
        a4580_extend(target);
    }
    return (seq_t)a4580_dig[n - 1];
}
