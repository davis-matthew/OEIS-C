/*
 * A065339  Number of primes congruent to 3 modulo 4 dividing n (with
 *          multiplicity).
 * https://oeis.org/A065339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime factors of n that are congruent to 3 (mod
 *            4), counted with multiplicity
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n < 2) {
        return 0;
    }

    uint64_t m = (uint64_t)n;
    seq_t result = 0;

    while (m > 1) {
        uint64_t p = smallest_prime_factor(m);
        if (p == 0) {          /* fallback: m itself is prime */
            p = m;
        }
        unsigned exp = 0;
        while (m % p == 0) {
            ++exp;
            m /= p;
        }
        if ((p % 4) == 3) {
            result += exp;
        }
    }

    return result;
}
