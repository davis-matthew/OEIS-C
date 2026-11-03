/*
 * A141160  Primes of the form -x^2 + 3*x*y + 3*y^2 (as well as of the form
 *          5*x^2 + 9*x*y + 3*y^2).
 * https://oeis.org/A141160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A141160: the n‑th prime p such that p = 3 or p ≡ 5,17,20 (mod 21). */
/* The function searches successive numbers, tests for primality with
   isPrimeGeneric, and counts those that fall into the required residue
   classes.  No caching or dynamic allocation is needed. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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
    /* candidate prime */
    idx_t cand = 3;
    /* how many valid primes we have already seen */
    idx_t count = 0;

    for (;; ++cand) {
        seq_t v = (seq_t)cand;
        if (isPrimeGeneric(v) &&
            (v == 3 || v % 21 == 5 || v % 21 == 17 || v % 21 == 20))
        {
            ++count;
            if (count == n)
                return v;
        }
    }
}
