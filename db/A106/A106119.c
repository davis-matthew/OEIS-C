/*
 * A106119  Primes with maximal digit > 2.
 * https://oeis.org/A106119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth prime whose decimal representation contains a
 *            digit ≥ 3 (all primes except 2, 11, 101, 211, …).
 *            Implementation: iterate through integers, test primality by
 *            trial division using only odd divisors up to the square root,
 *            check the digit condition, and count qualifying primes until
 *            the...
 * Range:     u8 n <= 50, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   has_digit_ge3_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_digit_ge3_uint64.h"

/* Helper: trial‑division primality test on a 64‑bit unsigned integer. */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2u) return false;
    if (x % 2u == 0u) return x == 2u;
    for (uint64_t p = 3u; p * p <= x; p += 2u)
        if (x % p == 0u) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;         /* undefined for n=0, but return 0 */

    // Count of qualifying primes found so far.
    idx_t found = 0;

    // Current candidate to test for primality.
    uint64_t candidate = 2u;

    while (true) {
        if (is_prime_uint64(candidate)) {
            if (has_digit_ge3_uint64(candidate)) {
                ++found;
                if (found == n) {
                    return (seq_t)candidate;
                }
            }
        }

        // Advance to the next candidate (skip even numbers after 2).
        if (candidate == 2u)
            candidate = 3u;
        else
            candidate += 2u;
    }
}
