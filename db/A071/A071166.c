/*
 * A071166  a(n) = n - A006530(A000203(n)), difference between n and largest
 *          prime factor of the sum of its divisors.
 * https://oeis.org/A071166
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - largest_prime_factor(sigma(n))
 * Range:     i8 n <= 59, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   largest_prime_factor, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);
    uint64_t sigma64 = (uint64_t)sigma;
    uint64_t lpf = largest_prime_factor(sigma64);
    return (seq_t)n - (seq_t)lpf;
}
