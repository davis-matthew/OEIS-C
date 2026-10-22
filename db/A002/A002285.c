/*
 * A002285  Decimal expansion of common logarithm of e.
 * https://oeis.org/A002285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n-th decimal digit of log_10(e) = 1/log(10); log(10) =
 *            46*atanh(1/31) + 34*atanh(1/49) + 20*atanh(1/161), digits of
 *            1/log(10) found by long division
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   c2285_cmp, c2285_iszero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/c2285_cmp.h"
#include "../utils/c2285_iszero.h"

#define B1E9 1000000000ULL

static void c2285_divs(uint32_t *x, size_t L, uint32_t d) {
    uint64_t rem = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t cur = rem * B1E9 + x[i];
        x[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static void c2285_muls(uint32_t *x, size_t L, uint64_t m) {
    uint64_t carry = 0;
    size_t i = L;
    while (i--) {
        uint64_t cur = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)(cur % B1E9);
        carry = cur / B1E9;
    }
}

static void c2285_add(uint32_t *a, const uint32_t *b, size_t L) {
    uint64_t carry = 0;
    size_t i = L;
    while (i--) {
        uint64_t cur = (uint64_t)a[i] + b[i] + carry;
        if (cur >= B1E9) { a[i] = (uint32_t)(cur - B1E9); carry = 1; }
        else { a[i] = (uint32_t)cur; carry = 0; }
    }
}

static void c2285_sub(uint32_t *a, const uint32_t *b, size_t L) {
    int64_t borrow = 0;
    size_t i = L;
    while (i--) {
        int64_t cur = (int64_t)a[i] - (int64_t)b[i] - borrow;
        if (cur < 0) { cur += (int64_t)B1E9; borrow = 1; }
        else borrow = 0;
        a[i] = (uint32_t)cur;
    }
}

static void c2285_atanh(uint32_t *acc, uint32_t *t, uint32_t *term, size_t L,
                        uint32_t x, uint32_t coef) {
    uint64_t k = 0;
    memset(t, 0, L * sizeof(uint32_t));
    t[0] = 1;
    c2285_divs(t, L, x);
    while (!c2285_iszero(t, L)) {
        memcpy(term, t, L * sizeof(uint32_t));
        c2285_divs(term, L, (uint32_t)(2 * k + 1));
        c2285_muls(term, L, coef);
        c2285_add(acc, term, L);
        c2285_divs(t, L, x * x);
        k++;
    }
}

static unsigned char *c2285_dig = NULL;
static idx_t c2285_cnt = 0;

static void c2285_compute(idx_t N) {
    size_t P = (size_t)((N + 40) / 9 + 4);
    size_t L = P + 1;
    uint32_t *ln = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *term = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *R = (uint32_t *)calloc(L, sizeof(uint32_t));
    idx_t j;

    c2285_atanh(ln, t, term, L, 31, 46);
    c2285_atanh(ln, t, term, L, 49, 34);
    c2285_atanh(ln, t, term, L, 161, 20);

    free(c2285_dig);
    c2285_dig = (unsigned char *)malloc((size_t)N);

    R[0] = 1;
    for (j = 0; j < N; j++) {
        uint64_t num, den, est;
        unsigned d;
        c2285_muls(R, L, 10);
        num = (uint64_t)R[0] * B1E9 + R[1];
        den = (uint64_t)ln[0] * B1E9 + ln[1] + 1;
        est = num / den;
        if (est) {
            memcpy(term, ln, L * sizeof(uint32_t));
            c2285_muls(term, L, est);
            c2285_sub(R, term, L);
        }
        d = (unsigned)est;
        while (c2285_cmp(R, ln, L) >= 0) {
            c2285_sub(R, ln, L);
            d++;
        }
        c2285_dig[j] = (unsigned char)d;
    }
    c2285_cnt = N;

    free(ln);
    free(t);
    free(term);
    free(R);
}

seq_t a(idx_t n) {
    if (n >= c2285_cnt) {
        idx_t N = c2285_cnt ? c2285_cnt : 1024;
        while (N <= n) N *= 2;
        c2285_compute(N);
    }
    return (seq_t)c2285_dig[n];
}
