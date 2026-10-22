/*
 * A006875  Non-seed mu-atoms of period n in Mandelbrot set.
 * https://oeis.org/A006875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d<n} phi(n/d) * Sum_{c|d} mu(d/c) * 2^(c-1)
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 65, u64 n <= 129,
 *            u128 n <= 257, big any n
 * Helpers:   pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pw2.h"

static int moebius(uint64_t n) {
    int sign = 1;
    uint64_t temp = n;
    for (uint64_t i = 2; i * i <= temp; i++) {
        if (temp % i == 0) {
            temp /= i;
            if (temp % i == 0) return 0;
            sign = -sign;
        }
    }
    if (temp > 1) sign = -sign;
    return sign;
}

static uint64_t phi(uint64_t n) {
    uint64_t result = n;
    uint64_t temp = n;
    for (uint64_t i = 2; i * i <= temp; i++) {
        if (temp % i == 0) {
            result = result / i * (i - 1);
            while (temp % i == 0) temp /= i;
        }
    }
    if (temp > 1) result = result / temp * (temp - 1);
    return result;
}

seq_t a(idx_t n) {
    uint64_t n64 = (uint64_t)n;
    seq_t total = 0;

    for (uint64_t d = 1; d < n64; d++) {
        if (n64 % d != 0) continue;

        seq_t pos = 0, neg = 0;
        for (uint64_t c = 1; c <= d; c++) {
            if (d % c != 0) continue;
            int mu_val = moebius(d / c);
            if (mu_val > 0) pos += pw2((idx_t)(c - 1));
            else if (mu_val < 0) neg += pw2((idx_t)(c - 1));
        }
        seq_t inner = pos - neg;
        total += (seq_t)phi(n64 / d) * inner;
    }

    return total;
}
