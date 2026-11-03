/*
 * A046431  Sum of digits of a(n) raised to its digits powers is prime.
 * https://oeis.org/A046431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 71 terms through n = 4772 (2026-11-03)
 * Helpers:   get_digits, ipow, is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes terms of OEIS A046431:
   a(n) is the n-th integer x ≥ 10 such that
   Σ(d in digits(x)) d^d is prime.  The implementation
   finds each term by brute force searching from the
   lowest candidate, without any caching. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/get_digits.h"
#include "../utils/ipow.h"
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

/* Maximum number of decimal digits expected to be seen while searching.
   256 digits is far more than needed for n ≤ 10 000. */
#define MAX_DIGITS 256

static seq_t digit_power_sum(seq_t x)
{
    uint8_t digits[MAX_DIGITS];
    size_t dlen = 0;
    get_digits(x, digits, &dlen);

    seq_t sum = 0;
    for (size_t i = 0; i < dlen; ++i) {
        seq_t d = (seq_t)digits[i];
        sum += ipow(d, d);          /* d^d */
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Search starting from the first admissible integer (10). */
    seq_t candidate = (seq_t)10;
    idx_t found = 0;                 /* number of terms found so far */

    for (;;) {
        seq_t s = digit_power_sum(candidate);

        if (isPrimeGeneric(s)) {
            if (found == n) {
                return candidate;
            }
            ++found;
        }
        candidate += (seq_t)1;       /* move to next integer */
    }
}
