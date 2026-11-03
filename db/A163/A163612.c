/*
 * A163612  Primes of form 5207*n + 1.
 * https://oeis.org/A163612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that p ≡ 1 (mod 5207)
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 937 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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
    /* a(1) is the first prime of the progression 5207·k + 1. */
    if (n == 0)
        return 0;               /* undefined for n < 1, return 0 as a guard */

    idx_t count = 0;            /* number of primes found so far   */
    idx_t k     = 1;            /* index in the arithmetic progression */

    while (count < n) {
        seq_t candidate = (seq_t)5207 * (seq_t)k + (seq_t)1;
        if (isPrimeGeneric(candidate)) {   /* primality test for seq_t */
            ++count;
            if (count == n)
                return candidate;         /* found the n-th term */
        }
        ++k;
    }

    /* Unreachable, but keep the compiler happy. */
    return 0;
}
