/*
 * A004606  Expansion of Pi in base 7.
 * https://oeis.org/A004606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, easy
 * Formula:   a(n) = n-th digit of Pi in base 7, with Pi = 16*arctan(1/5) -
 *            4*arctan(1/239) (Machin) in multiprecision binary fixed point,
 *            then converted to base 7
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9663 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void fp_add(uint32_t *x, const uint32_t *y, size_t L) {
    uint64_t c = 0;
    size_t i = L;
    while (i-- > 0) {
        uint64_t s = (uint64_t)x[i] + y[i] + c;
        x[i] = (uint32_t)s;
        c = s >> 32;
    }
}

/* x[0] is the integer part, x[1..L-1] are 32-bit fractional limbs */
static void fp_div(uint32_t *x, size_t L, uint32_t d) {
    uint64_t r = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static int fp_iszero(const uint32_t *x, size_t L) {
    size_t i;
    for (i = 0; i < L; i++) if (x[i]) return 0;
    return 1;
}

static void fp_sub(uint32_t *x, const uint32_t *y, size_t L) {
    int64_t b = 0;
    size_t i = L;
    while (i-- > 0) {
        int64_t s = (int64_t)x[i] - (int64_t)y[i] - b;
        if (s < 0) { s += ((int64_t)1 << 32); b = 1; } else b = 0;
        x[i] = (uint32_t)s;
    }
}

/* sum = arctan(1/m) */
static void fp_arctan_inv(uint32_t *sum, size_t L, uint32_t m) {
    uint32_t *term = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint64_t k = 0;
    memset(sum, 0, L * sizeof(uint32_t));
    term[0] = 1;
    fp_div(term, L, m);
    while (!fp_iszero(term, L)) {
        memcpy(t, term, L * sizeof(uint32_t));
        fp_div(t, L, (uint32_t)(2 * k + 1));
        if (k % 2 == 0) fp_add(sum, t, L); else fp_sub(sum, t, L);
        fp_div(term, L, m);
        fp_div(term, L, m);
        k++;
    }
    free(term);
    free(t);
}

static void fp_mul(uint32_t *x, size_t L, uint32_t m) {
    uint64_t c = 0;
    size_t i = L;
    while (i-- > 0) {
        uint64_t p = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)p;
        c = p >> 32;
    }
}

static unsigned char *pi7_digits = NULL;
static idx_t pi7_count = 0;

static void pi7_compute(idx_t N) {
    size_t bits = (size_t)N * 3 + 96;
    size_t L = bits / 32 + 3;
    uint32_t *p = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *q = (uint32_t *)calloc(L, sizeof(uint32_t));
    idx_t i;
    fp_arctan_inv(p, L, 5);
    fp_mul(p, L, 16);
    fp_arctan_inv(q, L, 239);
    fp_mul(q, L, 4);
    fp_sub(p, q, L);
    free(pi7_digits);
    pi7_digits = (unsigned char *)malloc((size_t)N);
    pi7_digits[0] = (unsigned char)p[0];
    for (i = 1; i < N; i++) {
        p[0] = 0;
        fp_mul(p, L, 7);
        pi7_digits[i] = (unsigned char)p[0];
    }
    free(p);
    free(q);
    pi7_count = N;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > pi7_count) {
        idx_t N = pi7_count * 2;
        if (N < n) N = n;
        if (N < 64) N = 64;
        pi7_compute(N);
    }
    return (seq_t)pi7_digits[n - 1];
}
