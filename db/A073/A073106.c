/*
 * A073106  Least k such that there are n primes among the numbers 2*n*x + 1
 *          for 1 <= x <= k.
 * https://oeis.org/A073106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = least k such that among numbers 1 + 2*n*x (1 ≤ x ≤ k)
 *            there are exactly n primes. (This is the index of the n-th prime
 *            in the arithmetic progression 1 (mod 2n).)
 * Range:     u8 n <= 7, u16 n <= 360, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Validated: standard, pass, 76 terms through n = 2294 (2026-11-03)
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
    idx_t primes_needed = n;   /* number of primes to find */
    idx_t k = 0;               /* current index x */
    idx_t primes_found = 0;    /* primes seen so far */

    while (primes_found < primes_needed) {
        ++k;
        /* Compute 1 + 2*n*k as seq_t (avoid overflow for large n/k). */
        seq_t val = (seq_t)2 * (seq_t)n * (seq_t)k + 1;
        if (isPrimeGeneric(val))
            ++primes_found;
    }
    return (seq_t)k;
}
