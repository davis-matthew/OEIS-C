/*
 * A094877  Primes that are reached in A094830.
 * https://oeis.org/A094877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   iterate x -> x + (sum of squares of digits of x) starting from n
 *            until x is prime; a(n) = that prime (search/iteration)
 * Range:     u8 n <= 7, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9460 (2026-11-07)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static inline uint64_t A094877_sumsq(uint64_t x)
{
    uint64_t s = 0;
    while (x != 0) {
        uint64_t d = x % 10;
        s += d * d;
        x /= 10;
    }
    return s;
}

seq_t a(idx_t n)
{
    uint64_t cur = (uint64_t)n;
    while (!is_prime(cur)) {
        cur += A094877_sumsq(cur);
    }
    return (seq_t)cur;
}
