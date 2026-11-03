/*
 * A088547  Primes of the form x^3+x^2+x+2.
 * https://oeis.org/A088547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 179, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 48 terms through n = 940 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form x^3 + x^2 + x + 2 */
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
    /* n is 1‑based index. Return the n‑th prime of the form
       x^3 + x^2 + x + 2. The search scans x from 0 upwards,
       tests the value with isPrimeGeneric, and counts primes. */
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    seq_t val;
    idx_t x = 0;
    idx_t count = 0;

    for (;; ++x)
    {
        seq_t xx = (seq_t)x * (seq_t)x;                     /* x^2   */
        val = xx * (seq_t)x + xx + (seq_t)x + (seq_t)2;     /* x^3 + x^2 + x + 2 */

        if (isPrimeGeneric(val))
        {
            ++count;
            if (count == n)
                return val;
        }
    }
}
