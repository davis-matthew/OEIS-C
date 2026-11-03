/*
 * A172989  Smallest k such that the two numbers n^2 +- k are primes.
 * https://oeis.org/A172989
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > 0 such that n^2 - k and n^2 + k are prime
 * Range:     u8 n <= 15, u16 n <= 706, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 9654 (2026-11-03)
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
    /* n is assumed to be >= 2, the sequence is defined only for those values. */
    seq_t sq = (seq_t)n * (seq_t)n;   /* compute n^2 in the unsigned type seq_t */

    for (seq_t k = 1; k < sq; ++k)
    {
        seq_t low  = sq - k;
        seq_t high = sq + k;

        if (isPrimeGeneric(low) && isPrimeGeneric(high))
            return k;                 /* smallest k found */
    }

    /* In theory this point is unreachable as a solution must exist. */
    return 0;
}
