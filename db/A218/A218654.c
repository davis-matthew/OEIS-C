/*
 * A218654  Number of ways to write n as x+y with 0<x<=y and x^2+3xy+y^2
 *          prime.
 * https://oeis.org/A218654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of x,y with 0 < x <= y, x + y = n and x^2 + 3xy +
 *            y^2 is prime
 * Range:     u8 n <= 1990, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 116 terms through n = 9446 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A218654: the number of x with 0 < x <= y, x + y = n and x^2 + 3xy + y^2
 * prime, tested with the exact 64-bit primality helper (the original divided
 * only by primes up to 12000, too few once the form passes 1.44e8). */

#include <stdint.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint64_t count = 0;
    for (uint64_t x = 1; x <= (uint64_t)n / 2; ++x) {
        uint64_t y = (uint64_t)n - x;
        if (is_prime_u64(x * x + 3 * x * y + y * y)) ++count;
    }
    return (seq_t)count;
}
