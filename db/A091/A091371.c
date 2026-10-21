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
 * Helpers:   _smallest_prime_factor, count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_smallest_prime_factor.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n) {
    if (n == 1) {
        /* By definition A091371(1) = 1 */
        return (seq_t)1;
    }
    seq_t lpf   = _smallest_prime_factor(n);    /* smallest prime factor */
    seq_t omega = count_prime_factors(n);        /* number of prime factors with multiplicity */
    return lpf - omega;
}
