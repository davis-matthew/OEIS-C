/*
 * A224925  a(n) = number of primes of the form p*q-(p+q) where p = prime(n)
 *          and q is any prime < p.
 * https://oeis.org/A224925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   an = number of primes of the form p*q-(p+q) where p = prime(n)
 *            and q is any prime < p
 * Range:     u8 n <= 7, u16 n <= 56, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include helper function prototypes */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
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

/* Static cache for primes up to the largest index requested */
static uint64_t *primes   = NULL;   /* array of prime numbers        */
static size_t   primes_cnt = 0;     /* number of primes in 'primes' */

/* Ensure that primes[0 .. n-1] contain the first n primes */
static void
ensure_primes(idx_t n)
{
    if (primes_cnt >= (size_t)n)
        return;                       /* already have enough primes */

    /* Free the previous array, if any */
    if (primes != NULL) {
        free(primes);
        primes   = NULL;
        primes_cnt = 0;
    }

    /* Find an upper bound for the nth prime and generate primes up to it */
    idx_t limit = _prime_limit_for_idx(n);
    build_primes_upto(limit, &primes, &primes_cnt);
}

/* Core function: compute a(n) */
seq_t
a(idx_t n)
{
    if (n == 0)           /* safety; AEIS offset is 1 */
        return (seq_t)0;

    ensure_primes(n);

    const uint64_t p = primes[n - 1];   /* nth prime */
    seq_t count = 0;

    for (idx_t i = 0; i < n - 1; ++i) {
        const uint64_t q = primes[i];

        /* m = p*q - (p+q) */
        seq_t m = ((seq_t)p) * ((seq_t)q)
                - (((seq_t)p) + ((seq_t)q));

        if (m < 2)                /* primes are >= 2 */
            continue;

        if (isPrimeGeneric(m))
            ++count;
    }

    return count;
}
