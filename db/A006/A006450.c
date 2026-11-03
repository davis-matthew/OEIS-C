/*
 * A006450  Prime-indexed primes: primes with prime subscripts.
 * https://oeis.org/A006450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = prime(prime(n))
 * Range:     u8 n <= 16, u16 n <= 844, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 67 terms through n = 9553 (2026-11-03)
 * Helpers:   prime_nth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A006450: prime(prime(n)), from a prime table that grows as needed (the
 * original sieved to a fixed 2000000 and returned 0 past it). */

#include <stdint.h>
#include "../utils/prime_nth.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    return (seq_t)prime_nth(prime_nth((uint64_t)n));
}
