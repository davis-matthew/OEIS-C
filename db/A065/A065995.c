/*
 * A065995  a(n) = prime(prime(n) + n).
 * https://oeis.org/A065995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(prime(n)+n)
 * Range:     u8 n <= 13, u16 n <= 759, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 65 terms through n = 9473 (2026-11-03)
 * Helpers:   prime_nth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A065995: prime(prime(n) + n), from a prime table that grows as needed (the
 * original sieved to a fixed 2000000 and returned 0 past it). */

#include <stdint.h>
#include "../utils/prime_nth.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    return (seq_t)prime_nth(prime_nth((uint64_t)n) + (uint64_t)n);
}
