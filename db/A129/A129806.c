/*
 * A129806  Primes congruent to +-5 mod 18.
 * https://oeis.org/A129806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime such that p % 18 == 5 or 13
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 969 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    idx_t found = 0;
    seq_t cand = 5;          /* first candidate (5 is the first term) */

    while (found < n) {
        if ((cand % 18 == 5) || (cand % 18 == 13)) {
            if (isPrimeGeneric(cand)) {
                ++found;
                if (found == n) return cand;
            }
        }
        cand += 2;           /* only odd numbers */
    }

    return 0; /* unreachable, but keeps the compiler happy */
}
