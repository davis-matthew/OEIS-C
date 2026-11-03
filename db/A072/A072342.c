/*
 * A072342  a(n) = the least natural number k such that k*reverse(n) + 1 is
 *          prime.
 * https://oeis.org/A072342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = least natural number k such that k*reverse(n)+1 is prime
 * Range:     u8 n <= 12, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Validated: partial, pass, 102 terms through n = 102 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, reverse_digits_idx
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
#include "../utils/reverse_digits_idx.h"

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
    /* No valid term for n < 1; the sequence is defined for n ≥ 1 */
    if (n < 1) return 0;

    /* reverse(n) is returned as an idx_t; cast it to seq_t for arithmetic */
    seq_t rev_n = (seq_t)reverse_digits_idx(n);

    for (idx_t k = 1; ; ++k) {
        seq_t candidate = rev_n * k + 1;          /* k*reverse(n)+1 */
        if (isPrimeGeneric(candidate))            /* primality test on seq_t */
            return (seq_t)k;                     /* a(n) is the first such k */
        /* The conjecture states a(n) ≤ n for all n, but the search is
           unbounded to avoid dependence on the conjecture. */
    }
}
