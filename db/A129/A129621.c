/*
 * A129621  Primes p such that at least one of k-1 and k+1 is prime, where k =
 *          sum of the two neighboring primes of 3*p.
 * https://oeis.org/A129621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   is_prime, is_prime_u128, is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A129621:
   * a(n) is the n-th prime p such that
   *   let m = 3*p,
   *   let p_left  = the largest prime < m,
   *   let p_right = the smallest prime > m,
   *   let k = p_left + p_right.
   *   At least one of k-1 or k+1 is prime.
   */

/* This implementation iterates over primes sequentially using
   the provided next_prime and prev_prime utilities.  It works
   for any build (fixed width or arbitrary precision) and
   respects the required external linkage. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
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

seq_t a(idx_t n) {
    /* Count of qualifying primes found so far.  Use unsigned long long
       so that count does not overflow for all required n (n up to ~10^4). */
    uint64_t count = 0;

    /* Start with the first prime 2.  The loop will process this prime
       and then advance to the next using next_prime, which returns the
       smallest prime strictly greater than its argument. */
    uint64_t p = 2;

    for (;;) {
        /* Compute 3*p; this fits in 64 bits for all p relevant to n ≤ 10000. */
        uint64_t m = 3 * p;

        /* Find neighboring primes of 3*p. */
        uint64_t p_left  = prev_prime(m);     /* largest prime < m  */
        uint64_t p_right = next_prime(m);     /* smallest prime > m */

        /* Sum of the neighbors. */
        uint64_t k = p_left + p_right;

        /* Check if k-1 or k+1 is prime.  Cast to seq_t because the
           isPrimeGeneric routine expects a seq_t argument. */
        if (isPrimeGeneric((seq_t)(k - 1)) || isPrimeGeneric((seq_t)(k + 1))) {
            ++count;
            if (count == n) {
                return (seq_t)p;   /* found the n-th term */
            }
        }

        /* Advance to the next prime.  next_prime(p) returns the next
           prime strictly greater than p. */
        p = next_prime(p);
    }
}
