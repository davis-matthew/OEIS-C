/*
 * A133765  Primes that contain the digit 4 or the digit 9.
 * https://oeis.org/A133765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 4368, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9699 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime that contains the decimal digit 4 or 9.
   Scans positive integers in order, uses is_prime(idx_t) to test
   primality, and a simple digit‑check to find the desired primes. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Return true if any decimal digit of x is 4 or 9. */
static bool contains49_idx(idx_t x)
{
    while (x > 0) {
        uint8_t d = x % 10;
        if (d == 4 || d == 9)
            return true;
        x /= 10;
    }
    return false;
}

/* Return the nth prime that contains the digit 4 or 9. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, but harmless */
        return 0;

    idx_t candidate = 2; /* candidate prime */
    idx_t found = 0;     /* how many qualifying primes seen */

    for (;; ++candidate) {
        if (is_prime(candidate) && contains49_idx(candidate)) {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
    }
}
