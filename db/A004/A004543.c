/*
 * A004543  Expansion of sqrt(2) in base 6.
 * https://oeis.org/A004543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(2) * 6^(n-1)) mod 6, computed by the base-6
 *            digit-by-digit square-root algorithm applied to 2 (memoized)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a4543_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/a4543_cmp.h"

#define A4543_LB 60466176ULL /* 6^10, limb base */

static unsigned char *a4543_dig = NULL;
static idx_t a4543_cnt = 0;

static void a4543_mul(uint64_t *x, size_t len, uint64_t m, uint64_t add) {
    uint64_t carry = add;
    size_t i;
    for (i = 0; i < len; i++) {
        uint64_t v = x[i] * m + carry;
        x[i] = v % A4543_LB;
        carry = v / A4543_LB;
    }
}

static void a4543_addsmall(uint64_t *x, size_t len, uint64_t c) {
    size_t i;
    for (i = 0; i < len && c; i++) {
        uint64_t v = x[i] + c;
        x[i] = v % A4543_LB;
        c = v / A4543_LB;
    }
}

static void a4543_sub(uint64_t *a, const uint64_t *b, size_t len) {
    uint64_t borrow = 0;
    size_t i;
    for (i = 0; i < len; i++) {
        uint64_t s = b[i] + borrow;
        if (a[i] >= s) { a[i] -= s; borrow = 0; }
        else { a[i] = a[i] + A4543_LB - s; borrow = 1; }
    }
}

static void a4543_compute(idx_t N) {
    size_t L = (size_t)(N / 10) + 6;
    uint64_t *P = (uint64_t *)calloc(L, sizeof(uint64_t));
    uint64_t *R = (uint64_t *)calloc(L, sizeof(uint64_t));
    uint64_t *T = (uint64_t *)calloc(L, sizeof(uint64_t));
    unsigned char *dig = (unsigned char *)malloc((size_t)N);
    idx_t i;
    for (i = 0; i < N; i++) {
        size_t len = (size_t)(i / 10) + 5;
        unsigned d = 0;
        if (len > L) len = L;
        /* bring down next pair of base-6 digits of 2 = (2).000... */
        a4543_mul(R, len, 36, i == 0 ? 2 : 0);
        /* T = 12*P + 1; subtract successive 12P + 2d + 1 while possible */
        memcpy(T, P, len * sizeof(uint64_t));
        a4543_mul(T, len, 12, 1);
        while (a4543_cmp(R, T, len) >= 0) {
            a4543_sub(R, T, len);
            a4543_addsmall(T, len, 2);
            d++;
        }
        a4543_mul(P, len, 6, d);
        dig[i] = (unsigned char)d;
    }
    free(P); free(R); free(T);
    free(a4543_dig);
    a4543_dig = dig;
    a4543_cnt = N;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > a4543_cnt) {
        idx_t N = 2 * n;
        if (N < 64) N = 64;
        a4543_compute(N);
    }
    return (seq_t)a4543_dig[n - 1];
}
