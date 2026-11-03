/*
 * A072341  a(n) = the least natural number k such that k*sigma(n) + 1 is
 *          prime.
 * https://oeis.org/A072341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least k such that k*sigma(n)+1 is prime
 * Range:     u8 n <= 15, u16 n <= 6737, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 119 terms through n = 9481 (2026-11-03)
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

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

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
    /* Ensure we only compute for valid indices; offset is 1. */
    if (n == 0)
        return 0;               /* undefined for n=0, return 0 */

    /* Compute σ(n).  The helper returns a seq_t value. */
    seq_t s = sigma_of(n);

    /* Find the smallest k such that k*s + 1 is prime. */
    seq_t k = 1;
    for (;; ++k) {
        seq_t v = s * k + 1;    /* v = σ(n)*k + 1 */
        if (isPrimeGeneric(v))
            return k;
    }
}
