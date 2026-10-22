/*
 * A002163  Decimal expansion of square root of 5.
 * https://oeis.org/A002163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(sqrt(5)*10^(n-1)) mod 10, computed by exact
 *            digit-by-digit square root (Jarvis subtraction method): a=5,
 *            b=1; for each digit d = number of times b can be subtracted from
 *            a (b += 2 after each subtraction); then a = 100*a, b = 10*b - 9.
 *            Digits are memoized with a doubling cache
 * Range:     u8 n <= 9680, u16 n <= 9680, u32 n <= 9680, u64 n <= 9680,
 *            u128 n <= 9680, big any n
 * Helpers:   s2163_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/s2163_cmp.h"

#define S2163_BASE 1000000000u

static unsigned char *s2163_dig = NULL;
static idx_t s2163_have = 0;

/* x -= y, requires x >= y */
static void s2163_sub(uint32_t *x, size_t *lx, const uint32_t *y, size_t ly) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < *lx; i++) {
        int64_t v = (int64_t)x[i] - borrow - (i < ly ? (int64_t)y[i] : 0);
        if (v < 0) { v += S2163_BASE; borrow = 1; } else borrow = 0;
        x[i] = (uint32_t)v;
        if (i >= ly && borrow == 0) break;
    }
    while (*lx > 1 && x[*lx - 1] == 0) (*lx)--;
}

static void s2163_addsmall(uint32_t *x, size_t *lx, uint32_t v) {
    uint64_t carry = v;
    size_t i = 0;
    while (carry) {
        if (i == *lx) { x[i] = 0; (*lx)++; }
        uint64_t t = (uint64_t)x[i] + carry;
        x[i] = (uint32_t)(t % S2163_BASE);
        carry = t / S2163_BASE;
        i++;
    }
}

static void s2163_subsmall(uint32_t *x, size_t *lx, uint32_t v) {
    int64_t borrow = v;
    size_t i = 0;
    while (borrow && i < *lx) {
        int64_t t = (int64_t)x[i] - borrow;
        if (t < 0) { t += S2163_BASE; borrow = 1; } else borrow = 0;
        x[i] = (uint32_t)t;
        i++;
    }
    while (*lx > 1 && x[*lx - 1] == 0) (*lx)--;
}

static void s2163_mulsmall(uint32_t *x, size_t *lx, uint32_t m) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < *lx; i++) {
        uint64_t t = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)(t % S2163_BASE);
        carry = t / S2163_BASE;
    }
    while (carry) {
        x[*lx] = (uint32_t)(carry % S2163_BASE);
        carry /= S2163_BASE;
        (*lx)++;
    }
    while (*lx > 1 && x[*lx - 1] == 0) (*lx)--;
}

static void s2163_compute(idx_t N) {
    size_t L = (size_t)(N / 9) + 8;
    uint32_t *A = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *B = (uint32_t *)calloc(L, sizeof(uint32_t));
    size_t la = 1, lb = 1;
    idx_t k;
    A[0] = 5;
    B[0] = 1;
    for (k = 0; k < N; k++) {
        unsigned d = 0;
        while (s2163_cmp(A, la, B, lb) >= 0) {
            s2163_sub(A, &la, B, lb);
            s2163_addsmall(B, &lb, 2);
            d++;
        }
        s2163_dig[k] = (unsigned char)d;
        s2163_mulsmall(A, &la, 100);
        s2163_mulsmall(B, &lb, 10);
        s2163_subsmall(B, &lb, 9);
    }
    free(A);
    free(B);
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > s2163_have) {
        idx_t target = s2163_have * 2;
        if (target < n) target = n;
        if (target < 64) target = 64;
        unsigned char *p = (unsigned char *)realloc(s2163_dig, (size_t)target);
        if (!p) return 0;
        s2163_dig = p;
        s2163_compute(target);
        s2163_have = target;
    }
    return (seq_t)s2163_dig[n - 1];
}
