/*
 * A142647  Primes congruent to 15 mod 56.
 * https://oeis.org/A142647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime congruent to 15 modulo 56. For n ≥ 1 the
 *            sequence is the increasing list of primes p such that p ≡ 15
 *            (mod 56)
 * Range:     u8 n <= 3, u16 n <= 270, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 972 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm tests successive candidates of the form 56·k+15
   and counts how many of them are prime until the desired rank is
   reached.  The routine is independent of previous calls, so it
   works correctly in any call order. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    /* For n < 1 the OEIS offset is irrelevant; return 0. */
    if (n == 0)
        return (seq_t)0;

    idx_t k = 0;          /* index of the current candidate 56·k+15 */
    seq_t count = 0;      /* number of primes found so far      */

    for (;;) {
        /* Candidate prime of the required form. */
        seq_t candidate = (seq_t)(56 * k + 15);

        /* Primality test for the candidate. */
        if (isPrimeGeneric(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++k;
    }
}
