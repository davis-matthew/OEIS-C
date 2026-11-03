/*
 * A001454  Number of permutations of length n with longest increasing
 *          subsequence of length 3.
 * https://oeis.org/A001454
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005802(n) - A000108(n), where u(n) = A005802(n)
 *            satisfies (n+2)^2*u(n) = (10*n^2+2*n-3)*u(n-1) -
 *            9*(n-1)^2*u(n-2), u(0) = 1, u(1) = 1, and Catalan C(n) =
 *            C(n-1)*2*(2n-1)/(n+1), C(0) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 46,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 820 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t a001454_big_calc(idx_t n) {
    seq_t u2 = 1, u1 = 1;
    for (idx_t k = 2; k <= n; k++) {
        seq_t K = (seq_t)k;
        seq_t P = K * K * 10 + K * 2 - 3;
        seq_t Q = (K - 1) * (K - 1) * 9;
        seq_t D = (K + 2) * (K + 2);
        seq_t u = (P * u1 - Q * u2) / D;
        u2 = u1;
        u1 = u;
    }
    seq_t c = 1;
    for (idx_t k = 1; k <= n; k++) {
        seq_t K = (seq_t)k;
        c = c * (K * 4 - 2) / (K + 1);
    }
    return u1 - c;
}

static void a001454_div(uint32_t *x, size_t L, uint64_t d) {
    uint64_t rem = 0;
    for (size_t i = L; i-- > 0;) {
        uint64_t cur = (rem << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static int a001454_fixed(void) {
    seq_t m = 0;
    m -= 1;
    return m > 0;
}

static void a001454_mul(uint32_t *x, size_t L, uint64_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t p = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)p;
        carry = p >> 32;
    }
}

static void a001454_sub(uint32_t *x, const uint32_t *y, size_t L) {
    uint64_t borrow = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t yi = (uint64_t)y[i] + borrow;
        if ((uint64_t)x[i] >= yi) {
            x[i] = (uint32_t)((uint64_t)x[i] - yi);
            borrow = 0;
        } else {
            x[i] = (uint32_t)(((uint64_t)1 << 32) + (uint64_t)x[i] - yi);
            borrow = 1;
        }
    }
}

static seq_t a001454_fixed_calc(idx_t n) {
    size_t L = (size_t)(n / 8 + 8);
    uint32_t *u2 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *u1 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *s = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *c = (uint32_t *)calloc(L, sizeof(uint32_t));
    u2[0] = 1;
    u1[0] = 1;
    for (idx_t k = 2; k <= n; k++) {
        memcpy(t, u1, L * sizeof(uint32_t));
        a001454_mul(t, L, (uint64_t)(10 * k * k + 2 * k - 3));
        memcpy(s, u2, L * sizeof(uint32_t));
        a001454_mul(s, L, (uint64_t)(9 * (k - 1) * (k - 1)));
        a001454_sub(t, s, L);
        a001454_div(t, L, (uint64_t)((k + 2) * (k + 2)));
        uint32_t *tmp = u2;
        u2 = u1;
        u1 = t;
        t = tmp;
    }
    c[0] = 1;
    for (idx_t k = 1; k <= n; k++) {
        a001454_mul(c, L, (uint64_t)(2 * (2 * k - 1)));
        a001454_div(c, L, (uint64_t)(k + 1));
    }
    a001454_sub(u1, c, L);
    seq_t r = 0;
    for (size_t i = L; i-- > 0;) {
        r = (seq_t)(r << 8);
        r = (seq_t)(r << 8);
        r = (seq_t)(r << 8);
        r = (seq_t)(r << 8);
        r += (seq_t)u1[i];
    }
    free(u2);
    free(u1);
    free(t);
    free(s);
    free(c);
    return r;
}

seq_t a(idx_t n) {
    if (n < 3) return 0;
    if (a001454_fixed()) return a001454_fixed_calc(n);
    return a001454_big_calc(n);
}
