/*
 * A232656  The number of pairs of numbers below n that, when generating a
 *          Fibonacci-like sequence modulo n, contain zeros.
 * https://oeis.org/A232656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d)*A001177(d), phi = A000010; A001177(d)
 *            found by iterating Fibonacci mod d
 * Range:     u8 n <= 18, u16 n <= 261, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 8085 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A232656_entry(uint64_t m) {
    if (m == 1) return 1;
    uint64_t f0 = 0, f1 = 1, k = 1;
    while (f1 != 0) {
        uint64_t f2 = f0 + f1;
        if (f2 >= m) f2 -= m;
        f0 = f1;
        f1 = f2;
        k++;
    }
    return k;
}

static uint64_t A232656_phi(uint64_t n) {
    uint64_t result = 1, m = n, p;
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            m /= p;
            result *= (p - 1);
            while (m % p == 0) {
                m /= p;
                result *= p;
            }
        }
    }
    if (m > 1) result *= (m - 1);
    return result;
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    seq_t total = 0;
    uint64_t d;
    for (d = 1; d <= N / d; d++) {
        if (N % d == 0) {
            uint64_t e = N / d;
            total += (seq_t)A232656_phi(d) * (seq_t)A232656_entry(d);
            if (e != d)
                total += (seq_t)A232656_phi(e) * (seq_t)A232656_entry(e);
        }
    }
    return total;
}
