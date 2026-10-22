/*
 * A007230  Number of free subsets of multiplicative group of GF(2^n).
 * https://oeis.org/A007230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) - A000016(m), m = odd part of n, where for odd m,
 *            A000016(m) = (1/(2m)) * Sum_{d|m} phi(d)*2^(m/d)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t tot(uint64_t x) {
    uint64_t r = x;
    for (uint64_t p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            while (x % p == 0) x /= p;
            r -= r / p;
        }
    }
    if (x > 1) r -= r / x;
    return r;
}

static seq_t two_pow(idx_t e) {
    seq_t r = 1;
    for (idx_t i = 0; i < e; i++) r *= 2;
    return r;
}

seq_t a(idx_t n) {
    idx_t m = n;
    while ((m & 1) == 0) m >>= 1;
    seq_t s = 0;
    for (idx_t d = 1; d <= m; d++) {
        if (m % d == 0) s += (seq_t)tot(d) * two_pow(m / d);
    }
    s /= (seq_t)m;
    s /= 2;
    return two_pow(n - 1) - s;
}
