/*
 * A091371  Smallest prime factor of n - number of prime factors of n with
 *          multiplicity.
 * https://oeis.org/A091371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = lpf(n) - Ω(n) where lpf(n) is the smallest prime factor
 *            of n and Ω(n) is the number of prime factors of n with
 *            multiplicity
 * Range:     i8 n <= 130, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 96 terms through n = 984 (2026-11-01)
 * Helpers:   bigomega_seq, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/bigomega_seq.h"
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n) {
    if (n == 1) {
        /* By definition A091371(1) = 1 */
        return (seq_t)1;
    }
    seq_t lpf   = smallest_prime_factor(n);    /* smallest prime factor */
    seq_t omega = bigomega_seq(n);        /* number of prime factors with multiplicity */
    return lpf - omega;
}
