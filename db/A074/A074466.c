/*
 * A074466  a(n) = gcd(n^3, sigma(n^3), phi(n^3)).
 * https://oeis.org/A074466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n^3, sigma(n^3), phi(n^3))
 * Range:     u8 n <= 5, u16 n <= 29, u32 n <= 1964, u64 n <= 8848,
 *            u128 n <= 8848, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Euclidean algorithm for seq_t */
static inline seq_t gcd_seq_t(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* fast exponentiation returning seq_t; exponent is idx_t */
static inline seq_t pow_uint_u64(uint64_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = (seq_t)base;
    while (exp) {
        if (exp & 1)
            result = result * b;
        b = b * b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* factor n (n <= 10000 for typical usage, simple trial division) */
    uint64_t fac[16];
    int exp[16];
    size_t fcnt = 0;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            fac[fcnt] = p;
            exp[fcnt] = e;
            ++fcnt;
        }
    }
    if (temp > 1) {
        fac[fcnt] = temp;
        exp[fcnt] = 1;
        ++fcnt;
    }

    /* n^3 */
    seq_t n3 = (seq_t)n * (seq_t)n * (seq_t)n;

    /* sigma(n^3) = product over primes of (p^{3e+1} - 1)/(p - 1) */
    seq_t sigma = 1;
    for (size_t i = 0; i < fcnt; ++i) {
        seq_t p_pow = pow_uint_u64(fac[i], 3 * exp[i] + 1);
        seq_t term = (p_pow - 1) / (fac[i] - 1);
        sigma *= term;
    }

    /* phi(n^3) = n^3 * ∏(1 - 1/p) = start with n3 and adjust each prime */
    seq_t phi = n3;
    for (size_t i = 0; i < fcnt; ++i) {
        phi = phi / fac[i] * (fac[i] - 1);
    }

    seq_t g1 = gcd_seq_t(n3, sigma);
    seq_t result = gcd_seq_t(g1, phi);
    return result;
}
