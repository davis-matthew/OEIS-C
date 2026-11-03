/*
 * A142204  Primes congruent to 7 mod 41.
 * https://oeis.org/A142204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 159, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 982 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime congruent to 7 (mod 41).
   For each n ≥ 1, returns the n‑th prime p with p ≡ 7 (mod 41). */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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
    if (n < 1)          /* undefined for n = 0, return 0 for safety */
        return 0;

    seq_t candidate = 7;   /* first prime in the progression */
    idx_t count     = 1;   /* number of primes found so far */

    if (n == 1)
        return candidate;

    for (;;) {
        candidate += 41;          /* move to the next number ≡ 7 (mod 41) */
        if (!isPrimeGeneric(candidate))
            continue;            /* skip composites */
        ++count;
        if (count == n)
            return candidate;
    }
}
