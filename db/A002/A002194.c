/*
 * A002194  Decimal expansion of sqrt(3).
 * https://oeis.org/A002194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn, easy
 * Formula:   digit-by-digit square root: a(n) = floor(sqrt(3)*10^(n-1)) mod
 *            10, with exact big-integer arithmetic (base 10^9 limbs); digits
 *            are cached
 * Range:     u8 n <= 9624, u16 n <= 9624, u32 n <= 9624, u64 n <= 9624,
 *            u128 n <= 9624, big any n
 * Helpers:   a002194_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/a002194_cmp.h"

#define A002194_BASE 1000000000u

static unsigned char *a002194_dig = NULL;
static idx_t a002194_have = 0;

static void a002194_muls(uint32_t *x, size_t m, uint32_t k) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < m; i++) {
        uint64_t t = (uint64_t)x[i] * k + carry;
        x[i] = (uint32_t)(t % A002194_BASE);
        carry = t / A002194_BASE;
    }
}

static void a002194_adds(uint32_t *x, size_t m, uint32_t v) {
    size_t i = 0;
    uint64_t carry = v;
    while (carry && i < m) {
        uint64_t t = (uint64_t)x[i] + carry;
        x[i] = (uint32_t)(t % A002194_BASE);
        carry = t / A002194_BASE;
        i++;
    }
}

static void a002194_sub(uint32_t *x, const uint32_t *y, size_t m) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < m; i++) {
        int64_t t = (int64_t)x[i] - (int64_t)y[i] - borrow;
        if (t < 0) { t += A002194_BASE; borrow = 1; } else borrow = 0;
        x[i] = (uint32_t)t;
    }
}

static void a002194_compute(idx_t target) {
    size_t L = (size_t)(target / 9 + 4);
    uint32_t *r = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *p = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *c = (uint32_t *)calloc(L, sizeof(uint32_t));
    unsigned char *d = (unsigned char *)malloc((size_t)target);
    idx_t i;
    for (i = 0; i < target; i++) {
        size_t m = (size_t)(i / 9 + 3);
        unsigned dd = 0;
        if (m > L) m = L;
        a002194_muls(r, m, 100);
        if (i == 0) a002194_adds(r, m, 3);
        memcpy(c, p, m * sizeof(uint32_t));
        a002194_muls(c, m, 20);
        a002194_adds(c, m, 1);
        while (a002194_cmp(r, c, m) >= 0) {
            a002194_sub(r, c, m);
            a002194_adds(c, m, 2);
            dd++;
        }
        a002194_muls(p, m, 10);
        a002194_adds(p, m, dd);
        d[i] = (unsigned char)dd;
    }
    free(r); free(p); free(c);
    free(a002194_dig);
    a002194_dig = d;
    a002194_have = target;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > a002194_have) {
        idx_t target = a002194_have * 2;
        if (target < n) target = n;
        if (target < 64) target = 64;
        a002194_compute(target);
    }
    return (seq_t)a002194_dig[n - 1];
}
