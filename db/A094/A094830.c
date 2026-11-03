/*
 * A094830  Start with x = n, repeatedly replace x with x + sum of squares of
 *          digits of x until you reach a prime; sequence gives number of
 *          steps.
 * https://oeis.org/A094830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9944 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A094830: a(n) = number of times the map x → x + sumsq(x) must be applied 
   to reach a prime from the starting value n. */

/* We use the helper from the utils library that tests primality of a 64‑bit
   unsigned integer.  The function is small enough that using a 64‑bit
   accumulator for the digit sum and the current value is safe for all
   index ranges that we are required to support.  The step counter is kept
   in the return type seq_t, so it automatically adapts to the target
   precision. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static inline uint64_t sumsq_digits(uint64_t x)
{
    uint64_t sum = 0;
    while (x != 0)
    {
        uint8_t d = (uint8_t)(x % 10);
        sum += (uint64_t)d * (uint64_t)d;
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    uint64_t cur = (uint64_t)n;   /* current value in the iteration   */
    seq_t steps = 0;              /* number of steps performed       */

    /* Iterate until a prime is reached.                     */
    while (!is_prime(cur))
    {
        cur += sumsq_digits(cur);
        ++steps;
    }
    return steps;
}
