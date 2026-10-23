/*
 * A107463  a(0)=0, a(n)=1 if n is 1 or is a prime, otherwise sum of prime
 *          factors of n with multiplicity.
 * https://oeis.org/A107463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 513, u16 n <= 9529, u32 n <= 9529, u64 n <= 9529,
 *            u128 n <= 9529, big any n
 * Helpers:   is_prime, sum_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A107463: a(0)=0, a(1)=1, a(n)=1 if n is 1 or prime,
   otherwise a(n) = sum of prime factors of n with multiplicity. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(uint64_t); */
#include "../utils/sum_prime_factors.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* a(n) = 1 for primes */
    if (is_prime((uint64_t)n)) return 1;

    /* otherwise return sum of prime factors with multiplicity */
    return sum_prime_factors((uint64_t)n);
}
