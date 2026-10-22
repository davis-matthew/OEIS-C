/*
 * A003814  Numbers k such that the continued fraction for sqrt(k) has odd
 *          period length.
 * https://oeis.org/A003814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th k (non-square) whose continued fraction for
 *            sqrt(k) has odd period length
 * Range:     u8 n <= 44, u16 n <= 7713, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t isqrt_u64(uint64_t x) {
    uint64_t r = 0, bit = (uint64_t)1 << 62;
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

static uint64_t cf_period_length(uint64_t k) {
    uint64_t a0 = isqrt_u64(k);
    if (a0 * a0 == k) return 0;
    uint64_t m = 0, d = 1, t = a0, period = 0;
    do {
        m = d * t - m;
        d = (k - m * m) / d;
        t = (a0 + m) / d;
        period++;
    } while (t != 2 * a0);
    return period;
}

static uint64_t *terms = NULL;
static uint64_t nterms = 0, cap = 0, lastk = 0;

seq_t a(idx_t n) {
    while (nterms < n) {
        lastk++;
        if (cf_period_length(lastk) % 2 == 1) {
            if (nterms == cap) {
                cap = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[nterms++] = lastk;
        }
    }
    return (seq_t)terms[n - 1];
}
