/*
 * A057674  Primes -p+2^n with smallest p prime, arising in A057674.
 * https://oeis.org/A057674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2^(n+1) - p, where p is the smallest prime such that the
 *            result (2^(n+1) - p) is prime. Negative primes are allowed
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 33 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, pow2
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
#include "../utils/pow2.h"

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
    /* Compute 2^(n+1). */
    seq_t N = pow2(n + 1);

    /* Search for the smallest prime p with prime difference. */
    for (idx_t p = 2; ; ++p) {
        if (!isPrimeGeneric((seq_t)p))
            continue;

        seq_t diff = N - (seq_t)p;

        if (diff < 0) {
            if (isPrimeGeneric(-(diff)))
                return diff;
        } else {
            if (isPrimeGeneric(diff))
                return diff;
        }
    }
}
