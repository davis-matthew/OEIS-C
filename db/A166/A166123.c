/*
 * A166123  If n is prime, a(n) = 1; otherwise, a(n) is gcd(n, d) where d is
 *          the denominator of the (n-1)-th Bernoulli number.
 * https://oeis.org/A166123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is prime; otherwise a(n) = gcd(n, d) where d is
 *            the denominator of the (n-1)-th Bernoulli number. Since d is the
 *            product of primes p with p-1 | (n-1), the gcd can be computed by
 *            testing each distinct prime factor p of n
 * Range:     u8 n <= 560, u16 n <= 7878, u32 n <= 7878, u64 n <= 7878,
 *            u128 n <= 7878, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Prime case */
    if (is_prime(n))
        return (seq_t)1;

    idx_t m = n - 1;

    /* Bernoulli numbers for odd indices >1 are zero → denominator 1 */
    if (m % 2 == 1)
        return (seq_t)1;

    seq_t result = 1;
    idx_t temp = n;

    /* Factorize n and test each distinct prime factor */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0)
                temp /= p;
            if (m % (p - 1) == 0)
                result *= (seq_t)p;
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        if (m % (temp - 1) == 0)
            result *= (seq_t)temp;
    }

    return result;
}
