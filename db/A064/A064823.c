/*
 * A064823  Number of indices 1 <= i <= n with A000796(i)=A000796(n).
 * https://oeis.org/A064823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = #{ 1 <= i <= n : A000796(i) = A000796(n) }, counted
 *            directly from the digits of Pi
 * Range:     u8 n <= 2303, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 113 terms through n = 8384 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A000796_BASE 1000000000ULL

static uint32_t *A000796_limbs = NULL;
static idx_t A000796_ndig = 0;

static void A000796_div(uint32_t *x, size_t s, size_t L, uint64_t d) {
    uint64_t r = 0;
    size_t i;
    for (i = s; i < L; i++) {
        uint64_t c = r * A000796_BASE + x[i];
        x[i] = (uint32_t)(c / d);
        r = c % d;
    }
}

static void A000796_addsub(uint32_t *res, const uint32_t *t, size_t s, size_t L, int add) {
    int64_t carry = 0;
    size_t i;
    for (i = L; i-- > 0;) {
        int64_t v = (int64_t)res[i] + carry;
        if (i >= s) v += add ? (int64_t)t[i] : -(int64_t)t[i];
        if (v < 0) { v += (int64_t)A000796_BASE; carry = -1; }
        else if (v >= (int64_t)A000796_BASE) { v -= (int64_t)A000796_BASE; carry = 1; }
        else carry = 0;
        res[i] = (uint32_t)v;
        if (i < s && carry == 0) break;
    }
}

static void A000796_atan(uint32_t *res, uint32_t *p, uint32_t *t, size_t L,
                         uint32_t coef, uint64_t m, int positive) {
    size_t s = 0;
    uint64_t k;
    memset(p, 0, L * sizeof(uint32_t));
    p[0] = coef;
    A000796_div(p, 0, L, m);
    for (k = 0;; k++) {
        while (s < L && p[s] == 0) s++;
        if (s == L) break;
        memcpy(t + s, p + s, (L - s) * sizeof(uint32_t));
        A000796_div(t, s, L, 2 * k + 1);
        A000796_addsub(res, t, s, L, ((k & 1) == 0) == (positive != 0));
        A000796_div(p, s, L, m * m);
    }
}

static void A000796_compute(idx_t ndig) {
    size_t L = (size_t)(ndig / 9) + 6;
    uint32_t *res = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *p = (uint32_t *)malloc(L * sizeof(uint32_t));
    uint32_t *t = (uint32_t *)malloc(L * sizeof(uint32_t));
    A000796_atan(res, p, t, L, 16, 5, 1);
    A000796_atan(res, p, t, L, 4, 239, 0);
    free(p);
    free(t);
    free(A000796_limbs);
    A000796_limbs = res;
    A000796_ndig = ndig;
}

static unsigned A000796_digit(idx_t n) {
    idx_t f, li, j;
    uint32_t v;
    if (n == 1) return (unsigned)A000796_limbs[0];
    f = n - 1;
    li = (f - 1) / 9 + 1;
    j = (f - 1) % 9;
    v = A000796_limbs[li];
    for (; j < 8; j++) v /= 10;
    return (unsigned)(v % 10);
}

seq_t a(idx_t n) {
    idx_t i;
    unsigned d;
    unsigned long long cnt = 0;
    if (n < 1) return 0;
    if (n > A000796_ndig) {
        idx_t want = n + 32;
        if (want < 2 * A000796_ndig) want = 2 * A000796_ndig;
        if (want < 1000) want = 1000;
        A000796_compute(want);
    }
    d = A000796_digit(n);
    for (i = 1; i <= n; i++) {
        if (A000796_digit(i) == d) cnt++;
    }
    return (seq_t)cnt;
}
