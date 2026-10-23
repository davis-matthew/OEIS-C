/*
 * A106101  Primes with minimal digit = 1.
 * https://oeis.org/A106101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th prime whose minimal decimal digit is 1 (i.e. the
 *            prime contains no 0s and its smallest digit equals 1)
 * Range:     u8 n <= 28, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   has_min_digit_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation scans integers from 2 upwards, tests primality and
   digit constraints, and returns the nth such prime.  It does not use
   dynamic memory, so it works both for fixed‑width integer seq_t and for
   arbitrary‑precision types. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_min_digit_one.h"

/* Simple deterministic primality test for 64‑bit integers. */
static bool isPrime64(uint64_t x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if ((x & 1ULL) == 0) return false;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* The sequence function. */
seq_t a(idx_t n)
{
    /* Behaviour for n < 1 is irrelevant; return 0. */
    if (n == 0) return 0;

    uint64_t count = 0;
    uint64_t candidate = 2;   /* start from the smallest prime */

    for (;; ++candidate) {
        if (!isPrime64(candidate)) continue;
        if (!has_min_digit_one(candidate)) continue;

        ++count;
        if (count == n) return (seq_t)candidate;
    }
}
