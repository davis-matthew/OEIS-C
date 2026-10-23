/*
 * A036270  9th differences of primes.
 * https://oeis.org/A036270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ninth forward difference of the prime numbers. That is,
 *            a(n) = Σ_{j=0}^9 (-1)^{9-j} * C(9,j) * p_{n+j}, where p_k is the
 *            k-th prime. Offset = 1
 * Range:     i8 none, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t primes[10];
    for (idx_t i = 0; i < 10; ++i)
        primes[i] = (seq_t)get_prime(n + i);

    for (int k = 1; k <= 9; ++k) {
        size_t len = 10 - k;
        for (size_t i = 0; i < len; ++i)
            primes[i] = primes[i + 1] - primes[i];
    }
    return primes[0];
}
