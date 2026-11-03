/*
 * A127356  a(n) is the smallest k > 0 such that k^2 + prime(n) is prime.
 * https://oeis.org/A127356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest k > 0 such that k^2 + prime(n) is prime
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 96 terms through n = 905 (2026-11-03)
 * Helpers:   is_prime_u64, prime_nth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A127356: the least k > 0 with k^2 + prime(n) prime, with a growing prime
 * table and the exact 64-bit primality test. */

#include <stdint.h>
#include "../utils/is_prime_u64.h"
#include "../utils/prime_nth.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint64_t p = prime_nth((uint64_t)n);
    for (uint64_t k = 1;; ++k)
        if (is_prime_u64(k * k + p)) return (seq_t)k;
}
