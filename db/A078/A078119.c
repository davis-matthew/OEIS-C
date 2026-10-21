/*
 * A078119  a(n) = sigma(n) - phi(n+2), where sigma = sum of divisors
 *          (A000203) and phi = Euler totient function (A000010).
 * https://oeis.org/A078119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma(n) - phi(n+2)
 * Range:     i8 n <= 59, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Sum of divisors σ(n) for a uint64_t n */
static seq_t sigma_uint64(uint64_t n) {
    if (n == 0) {
        return (seq_t)0;
    }
    uint64_t m = n;
    seq_t sigma = (seq_t)1;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int exp = 0;
            seq_t term = (seq_t)1;
            seq_t pow   = (seq_t)1;
            while (m % p == 0) {
                m /= p;
                ++exp;
                pow *= (seq_t)p;        /* p^exp part */
                term += pow;           /* sum of geometric series */
            }
            sigma *= term;
        }
    }
    if (m > 1) {                       /* remaining prime factor */
        sigma *= (seq_t)(1 + m);
    }
    return sigma;
}

/* Euler's totient φ(n) for a uint64_t n */
static seq_t phi_uint64(uint64_t n) {
    if (n == 0) {
        return (seq_t)0;
    }
    uint64_t m = n;
    seq_t phi = (seq_t)n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            phi = (phi / (seq_t)p) * ((seq_t)p - (seq_t)1);
            while (m % p == 0) {
                m /= p;
            }
        }
    }
    if (m > 1) {                       /* remaining prime factor */
        phi = (phi / (seq_t)m) * ((seq_t)m - (seq_t)1);
    }
    return phi;
}

/* External function for OEIS sequence A078119 */
seq_t a(idx_t n) {
    seq_t sigma_n   = sigma_uint64((uint64_t)n);
    seq_t phi_nplus = phi_uint64((uint64_t)(n + 2));
    return sigma_n - phi_nplus;
}
