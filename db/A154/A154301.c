/*
 * A154301  Primes of the form (1+2+...+m)/75 = A000217(m)/75.
 * https://oeis.org/A154301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy, bref
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: full, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A154301: primes of the form (1+2+…+m)/75 = m(m+1)/150.
 *  The set of such primes is finite; for m>150 the value cannot be prime.
 *  This implementation pre‑computes all candidates once and stores the
 *  primes in a small static cache.  Subsequent calls return the cached
 *  value for the given n or 0 if n exceeds the number of known primes.
 */

#include <stdbool.h>
#include <stdint.h>

/* Provided by the framework for primality testing. */
#include <stddef.h>
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

/* static cache of the primes found */
static seq_t cache[4] = {0, 0, 0, 0};
static idx_t cache_count = 0;
static bool cache_initialized = false;

/* pre‑compute all primes of the form m(m+1)/150 for 1 ≤ m ≤ 150 */
static void precompute(void)
{
    const idx_t M_LIMIT = 150;
    for (idx_t m = 1; m <= M_LIMIT; ++m) {
        unsigned long long prod = m * (m + 1);            /* m(m+1)  */
        if (prod % 150 != 0) continue;                   /* not integral */
        unsigned long long val = prod / 150;             /* candidate prime */
        seq_t p = (seq_t)val;                            /* safe cast – val < 2^15 */
        if (isPrimeGeneric(p)) {
            if (cache_count < 4) {                       /* store only first 4 */
                cache[cache_count++] = p;
            } else {
                /* more primes would overflow our small cache; ignore */
            }
        }
    }
    cache_initialized = true;
}

/* ---------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* behaviour below the offset is irrelevant */

    if (!cache_initialized) precompute();

    /* n is 1‑based */
    if (n <= cache_count) return cache[n - 1];
    return (seq_t)0;                     /* no further terms */
}
