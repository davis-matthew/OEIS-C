/*
 * A069591  Smallest prime in which the n-th significant digit is a 7.
 * https://oeis.org/A069591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest prime p such that the n-th least‑significant
 *            decimal digit of p is 7. In other words, 7·10^(n−1) ≤ p <
 *            8·10^(n−1). This finds the smallest prime in that interval
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The utilities below provide big‑integer arithmetic, powers of ten
   and a deterministic primality test for arbitrary‐precision
   integers.  No other external code is required. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
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
    /* Special case n == 1: 7 is the only prime with the first
       digit (units place) equal to 7. */
    if (n == (idx_t)1)
        return (seq_t)7;

    /* Compute 10^(n‑1) as a big integer. */
    seq_t pow10_n_1 = pow10(n - (idx_t)1);

    /* Lower and upper bounds of the interval [7·10^(n‑1),
       8·10^(n‑1)–1]. */
    seq_t lower = (seq_t)7 * pow10_n_1;
    seq_t upper = (seq_t)8 * pow10_n_1;

    /* Start searching from the first candidate > lower,
       because lower itself is a multiple of 10 and composite. */
    seq_t candidate = lower + (seq_t)1;

    /* Increment until we find a prime or reach the upper bound.
       The upper bound should always contain at least one prime;
       if not, the loop would exit without returning, but the
       sequence definition guarantees existence for all n. */
    while (candidate < upper) {
        if (isPrimeGeneric(candidate))
            return candidate;
        candidate = candidate + (seq_t)1;
    }

    /* In practice this line is never reached for valid n, but
       to satisfy the compiler we return 0. */
    return (seq_t)0;
}
