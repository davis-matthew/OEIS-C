/*
 * A107393  a(n) = -1 if n is a prime, else a(n) = 1 if n is the sum of three
 *          odd primes, else a(n) = 2 if n is the sum of two primes, else a(n)
 *          = 0.
 * https://oeis.org/A107393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, less
 * Formula:   a(n) = -1 if n is a prime, 1 if n is the sum of three odd
 *            primes, 2 if n is the sum of two primes, else 0
 * Range:     i8 n <= 8811, i16 n <= 8811, i32 n <= 8811, i64 n <= 8811,
 *            i128 n <= 8811, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* 0 and 1 are not primes and not sums of primes. */
    if (n <= 1)
        return 0;

    /* Prime test. */
    if (is_prime((uint64_t)n))
        return (seq_t)-1;

    /* Sum of three odd primes: every odd n ≥ 9 can be written as such. */
    if ((n & 1ULL) && (n >= 9))
        return (seq_t)1;

    /* Sum of two primes (Goldbach check). */
    for (idx_t p = 2; p <= n / 2; ++p) {
        if (is_prime((uint64_t)p) && is_prime((uint64_t)(n - p)))
            return (seq_t)2;
    }

    return 0;
}
