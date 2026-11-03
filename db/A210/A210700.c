/*
 * A210700  A047160(3n): smallest m >= 0 with both 3n - m and 3n + m prime.
 * https://oeis.org/A210700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 3333, u32 n <= 3333, u64 n <= 3333,
 *            u128 n <= 3333, big any n
 * Validated: standard, pass, 98 terms through n = 3096 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal m≥0 such that 3n - m and 3n + m are primes */
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
    if (n == 1)
        return 0;                     /* 3*1 = 3 is prime */

    seq_t base = (seq_t)3 * (seq_t)n; /* 3n  (seq_t guarantees enough width) */

    /* For even n, m must be odd; for odd n, m must be even. */
    idx_t m = (n & 1) ? 2 : 1;        /* start value */

    for (;; m += 2)                   /* increase by 2 to keep the parity */
    {
        if (m % 3 == 0)               /* skip multiples of 3 */
            continue;

        seq_t left  = base - (seq_t)m;
        seq_t right = base + (seq_t)m;

        if (isPrimeGeneric(left) && isPrimeGeneric(right))
            return (seq_t)m;         /* found the minimal m */
    }
}
