/*
 * A142795  Primes congruent to 43 mod 60.
 * https://oeis.org/A142795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime congruent to 43 (mod 60), OEIS A142795
 * Range:     u8 n <= 4, u16 n <= 401, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 783 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
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

/* Return the n-th prime ≡ 43 (mod 60).  For n == 0 an unspecified value
   (0) is returned for safety; the sequence is defined only for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but harmless */
    idx_t found = 0;                      /* how many primes found so far */
    idx_t k = 0;                          /* counter for numbers 60·k+43 */

    for (;; ++k) {
        /* Compute the candidate as an idx_t to avoid overflow. */
        idx_t value = 43 + (idx_t)60 * k;
        seq_t candidate = (seq_t)value;

        /* Use the provided generic primality test. */
        if (isPrimeGeneric(candidate)) {
            ++found;
            if (found == n) return candidate;
        }
    }
}
