/*
 * A107463  a(0)=0, a(n)=1 if n is 1 or is a prime, otherwise sum of prime
 *          factors of n with multiplicity.
 * https://oeis.org/A107463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 513, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 98 terms through n = 9529 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A107463: a(0)=0, a(1)=1, a(n)=1 if n is 1 or prime,
   otherwise a(n) = sum of prime factors of n with multiplicity. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

/* Return the sum of the prime factors of m, counting multiplicity. */
static seq_t sum_prime_factors(uint64_t m)
{
    seq_t sum = 0;

    /* factor out 2's */
    while ((m & 1ULL) == 0) {
        sum += (seq_t)2;
        m >>= 1;
    }

    /* odd factors */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        while (m % p == 0) {
            sum += (seq_t)p;
            m /= p;
        }
    }

    /* remaining prime > 1 */
    if (m > 1) {
        sum += (seq_t)m;
    }

    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* a(n) = 1 for primes */
    if (is_prime_u64((uint64_t)n)) return 1;

    /* otherwise return sum of prime factors with multiplicity */
    return sum_prime_factors((uint64_t)n);
}
