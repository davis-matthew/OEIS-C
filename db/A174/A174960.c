/*
 * A174960  Smallest prime p such that p + n*(n+1)/2 is prime, or 0 if no such
 *          prime exists.
 * https://oeis.org/A174960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that p + n*(n+1)/2 is prime, or 0
 *            if no such prime exists
 * Range:     u8 n <= 22, u16 n <= 1242, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9202 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* Compute triangular number T = n*(n+1)/2 in seq_t */
    seq_t T = (seq_t)n * ((seq_t)n + 1) / 2;

    /* If T is odd, only p = 2 can make p + T odd; test it */
    if ((T & 1) != 0) {
        return isPrimeGeneric(2 + T) ? (seq_t)2 : (seq_t)0;
    }

    /* Even triangular number: search for the smallest prime p with p + T prime */
    for (seq_t p = 2; ; ++p) {
        if (!isPrimeGeneric(p))
            continue;
        if (isPrimeGeneric(p + T))
            return p;
    }
}
