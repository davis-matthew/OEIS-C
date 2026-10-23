/*
 * A102440  Replace each prime factor of n that is greater than 3 with the
 *          greatest semiprime less than it.
 * https://oeis.org/A102440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102440: a(n) = product of prime factors of n,
   replacing each prime factor p>3 by the greatest semiprime < p
   (semiprime = product of two primes, not necessarily distinct).  */
/* Implementation relies on the helper is_prime_uint64() found in
   "../utils/is_prime_uint64.h". */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

/* Return true if x is a semiprime: product of exactly two primes. */
static bool is_semiprime(uint64_t x)
{
    for (uint64_t d = 2; d * d <= x; ++d) {
        if (x % d == 0) {
            uint64_t y = x / d;
            /* Check that both divisors are prime */
            return is_prime_uint64(d) && is_prime_uint64(y);
        }
    }
    /* x has no divisor <= sqrt(x) -> prime, not semiprime */
    return false;
}

/* Largest semiprime strictly less than prime p (>3).  Assumes p > 3. */
static seq_t largest_semiprime_lt(uint64_t p)
{
    for (uint64_t x = p - 1; x >= 4; --x) {
        if (is_semiprime(x))
            return (seq_t)x;
        if (x == 4)  /* 4 is the smallest semiprime; safety break */
            break;
    }
    /* Should never reach here for p > 3 */
    return (seq_t)0;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;           /* a(1) = 1 */

    uint64_t nn = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* Factor n by trial division */
    for (uint64_t d = 2; d * d <= nn; ++d) {
        if (nn % d == 0) {
            uint64_t exp = 0;
            while (nn % d == 0) {
                ++exp;
                nn /= d;
            }
            seq_t factor = (d <= 3) ? (seq_t)d
                                    : largest_semiprime_lt(d);
            for (uint64_t i = 0; i < exp; ++i)
                result *= factor;
        }
    }
    /* Remaining prime factor > sqrt(original n) */
    if (nn > 1) {
        seq_t factor = (nn <= 3) ? (seq_t)nn
                                 : largest_semiprime_lt(nn);
        result *= factor;
    }

    return result;
}
