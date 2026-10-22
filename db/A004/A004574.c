/*
 * A004574  Expansion of sqrt(7) in base 7.
 * https://oeis.org/A004574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   digit-by-digit square root in base 7: p = 2, r = 3; then for
 *            each step, r := 49*r, d = max{d in 0..6 : (14*p + d)*d <= r}, r
 *            := r - (14*p + d)*d, p := 7*p + d; a(1) = 2, a(n) = d produced
 *            at step n-1 (exact big-integer arithmetic internally)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bn_add_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bn_add_small.h"

static void bn_mul_small(uint32_t *a, size_t len, uint32_t m) {
    uint64_t c = 0;
    for (size_t i = 0; i < len; i++) {
        uint64_t v = (uint64_t)a[i] * m + c;
        a[i] = (uint32_t)v;
        c = v >> 32;
    }
}

static int bn_cmp(const uint32_t *a, const uint32_t *b, size_t len) {
    size_t i = len;
    while (i > 0) {
        i--;
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

static void bn_sub(uint32_t *a, const uint32_t *b, size_t len) {
    int64_t borrow = 0;
    for (size_t i = 0; i < len; i++) {
        int64_t s = (int64_t)a[i] - (int64_t)b[i] - borrow;
        if (s < 0) { s += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)s;
    }
}

static unsigned char *a4574_digits = NULL;
static idx_t a4574_count = 0;

static void a4574_compute(idx_t N) {
    size_t L = (size_t)(N / 11 + 8);
    uint32_t *p = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *y = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    unsigned char *dg = (unsigned char *)malloc((size_t)N + 1);

    p[0] = 2;
    r[0] = 3;
    dg[0] = 2;
    for (idx_t k = 1; k < N; k++) {
        size_t al = (size_t)(k / 11 + 5);
        if (al > L) al = L;
        bn_mul_small(r, al, 49);
        memcpy(y, p, al * sizeof(uint32_t));
        bn_mul_small(y, al, 14);
        uint32_t d = 0;
        for (uint32_t td = 6; td >= 1; td--) {
            memcpy(t, y, al * sizeof(uint32_t));
            bn_add_small(t, al, td);
            bn_mul_small(t, al, td);
            if (bn_cmp(t, r, al) <= 0) { d = td; break; }
        }
        if (d > 0) bn_sub(r, t, al);
        bn_mul_small(p, al, 7);
        bn_add_small(p, al, d);
        dg[k] = (unsigned char)d;
    }

    free(p); free(r); free(y); free(t);
    free(a4574_digits);
    a4574_digits = dg;
    a4574_count = N;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (n > a4574_count) {
        idx_t target = a4574_count * 2;
        if (target < n) target = n;
        if (target < 256) target = 256;
        a4574_compute(target);
    }
    return (seq_t)a4574_digits[n - 1];
}
