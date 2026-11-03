/*
 * A140535  Primes congruent to 1 or 209 mod 210.
 * https://oeis.org/A140535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 270, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 931 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function finds the nth prime congruent to 1 or 209 modulo 210. */
/* No caching is used; the function recomputes each value independently. */
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
    if (n == 0)
        return (seq_t)0;           /* undefined for n < 1, guard only */

    idx_t found = 0;               /* number of suitable primes found so far */
    seq_t cand  = (seq_t)211;      /* first prime that satisfies the condition */

    const seq_t mod = (seq_t)210;  /* modulus for (cand mod 210) */

    /* search for primes in increasing order */
    while (true) {
        /* Check the two allowed residue classes modulo 210 */
        if (cand % mod == (seq_t)1 || cand % mod == (seq_t)209) {
            if (isPrimeGeneric(cand)) {
                ++found;
                if (found == n)
                    return cand;
            }
        }
        cand += (seq_t)2;          /* skip even numbers */
    }
}
