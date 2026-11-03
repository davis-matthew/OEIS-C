/*
 * A129213  Numbers m such that 7!*m-/+1 are twin primes.
 * https://oeis.org/A129213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9910 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A129213: a(n) = m such that 7!*m ± 1 are twin primes.
   The function scans m from 1 upward, testing 5040*m-1 and 5040*m+1
   for primality using isPrimeGeneric(). It stops when the nth such m
   is found. */

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
    if (n == 0) return 0;          /* offset is 1 */

    seq_t m = 1;
    idx_t count = 0;

    while (true) {
        seq_t v  = (seq_t)5040 * (seq_t)m;
        seq_t a1 = v - 1;
        seq_t a2 = v + 1;

        if (isPrimeGeneric(a1) && isPrimeGeneric(a2)) {
            count++;
            if (count == n) return m;
        }
        m++;
    }
}
