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
 * Validated: standard, pass, 103 terms through n = 9183 (2026-11-01)
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/largest_prime_factor.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);
    uint64_t sigma64 = (uint64_t)sigma;
    uint64_t lpf = largest_prime_factor(sigma64);
    return (seq_t)n - (seq_t)lpf;
}
