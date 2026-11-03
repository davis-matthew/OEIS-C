/*
 * A004559  Expansion of sqrt(5) in base 6.
 * https://oeis.org/A004559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(5) * 6^(n-1)) mod 6, computed exactly as the
 *            integer square root isqrt(5 * 36^(n-1)) using bit-by-bit
 *            big-integer arithmetic
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 115 terms through n = 4000 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void big_add_bit(uint32_t *x, size_t L, size_t b) {
    size_t i = b / 32;
    uint64_t carry = (uint64_t)1 << (b % 32);
    while (carry && i < L) {
        uint64_t s = (uint64_t)x[i] + carry;
        x[i] = (uint32_t)s;
        carry = s >> 32;
        i++;
    }
}

static int big_cmp(const uint32_t *x, const uint32_t *y, size_t L) {
    size_t i = L;
    while (i > 0) {
        i--;
        if (x[i] != y[i]) return x[i] > y[i] ? 1 : -1;
    }
    return 0;
}

static void big_shr1(uint32_t *x, size_t L) {
    size_t i;
    for (i = 0; i < L; i++) {
        uint32_t hi = (i + 1 < L) ? x[i + 1] : 0;
        x[i] = (x[i] >> 1) | (hi << 31);
    }
}

static void big_sub(uint32_t *x, const uint32_t *y, size_t L) {
    uint64_t borrow = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t yi = (uint64_t)y[i] + borrow;
        if ((uint64_t)x[i] >= yi) {
            x[i] = (uint32_t)((uint64_t)x[i] - yi);
            borrow = 0;
        } else {
            x[i] = (uint32_t)(((uint64_t)1 << 32) + x[i] - yi);
            borrow = 1;
        }
    }
}

static uint8_t *dig = NULL;
static idx_t ndig = 0;

static void compute(idx_t N) {
    idx_t m = N - 1;
    size_t bits = (size_t)((m * 26) / 5 + 8);
    size_t L = bits / 32 + 2;
    uint32_t *num = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *res = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    idx_t i;
    size_t j, top, msb, b;

    num[0] = 5;
    for (i = 0; i < m; i++) {
        uint64_t carry = 0;
        for (j = 0; j < L; j++) {
            uint64_t p = (uint64_t)num[j] * 36 + carry;
            num[j] = (uint32_t)p;
            carry = p >> 32;
        }
    }

    top = L - 1;
    while (top > 0 && num[top] == 0) top--;
    msb = top * 32;
    {
        uint32_t v = num[top];
        int k = 0;
        while (v >> 1) { v >>= 1; k++; }
        msb += (size_t)k;
    }
    b = msb & ~(size_t)1;

    for (;;) {
        memcpy(t, res, L * sizeof(uint32_t));
        big_add_bit(t, L, b);
        if (big_cmp(num, t, L) >= 0) {
            big_sub(num, t, L);
            big_shr1(res, L);
            big_add_bit(res, L, b);
        } else {
            big_shr1(res, L);
        }
        if (b < 2) break;
        b -= 2;
    }

    {
        uint8_t *nd = (uint8_t *)malloc((size_t)N);
        const uint64_t D = 2176782336ULL; /* 6^12 */
        idx_t produced = 0;
        while (produced < N) {
            uint64_t rem = 0;
            int k;
            j = L;
            while (j > 0) {
                j--;
                uint64_t cur = (rem << 32) | res[j];
                res[j] = (uint32_t)(cur / D);
                rem = cur % D;
            }
            for (k = 0; k < 12 && produced < N; k++) {
                nd[N - 1 - produced] = (uint8_t)(rem % 6);
                rem /= 6;
                produced++;
            }
        }
        free(dig);
        dig = nd;
        ndig = N;
    }

    free(num);
    free(res);
    free(t);
}

seq_t a(idx_t n) {
    if (n > ndig) {
        idx_t N = n;
        if (N < 2 * ndig) N = 2 * ndig;
        if (N < 64) N = 64;
        compute(N);
    }
    return (seq_t)dig[n - 1];
}
