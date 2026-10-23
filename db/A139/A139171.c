/*
 * A139171  a(n) = smallest prime number p such that p!/n is an integer.
 * https://oeis.org/A139171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = smallest prime p such that p! is divisible by n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the definition directly using Legendre's formula for
 * exponents of primes in factorials. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides is_prime(idx_t) */
#include "../utils/sieve_primes.h" /* provides utility prototypes */

/* Compute exponent of prime q in p! using Legendre's formula. */
static uint64_t legendre(uint64_t p, uint64_t q)
{
    uint64_t sum = 0;
    while (p >= q) {
        p /= q;
        sum += p;
    }
    return sum;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                 /* undefined for n < 1 */

    uint64_t m = (uint64_t)n;                   /* copy of n for factoring */
    uint64_t threshold = 2;                     /* minimal prime that works */
    uint64_t i = 2;

    /* trial division up to sqrt(m) */
    while (i * i <= m) {
        if (m % i == 0) {
            uint64_t exp = 0;
            while (m % i == 0) {
                m /= i;
                ++exp;
            }
            /* find smallest prime q >= i with exponent >= exp in q! */
            uint64_t p = i;
            while (legendre(p, i) < exp) ++p;
            if (p > threshold) threshold = p;
        }
        ++i;
    }

    /* Remaining factor > sqrt(n) is prime. */
    if (m > 1) {
        if (m > threshold) threshold = m;
    }

    /* Find the smallest *prime* ≥ threshold. */
    uint64_t candidate = threshold;
    while (!is_prime(candidate)) ++candidate;

    return (seq_t)candidate;
}
