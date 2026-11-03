/*
 * A229980  Smallest integer m > 0 such that p(n) + m*p(n + 1) and m*p(n) +
 *          p(n + 1) are prime, p(n) = n-th prime.
 * https://oeis.org/A229980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m > 0 such that p(n)+m*p(n+1) and m*p(n)+p(n+1)
 *            are prime
 * Range:     u8 n <= 6, u16 n <= 97, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 117 terms through n = 945 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
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

static uint64_t *prime_list = NULL;   /* cache of primes */
static size_t prime_count = 0;        /* number of cached primes */

/* Ensure that the cache contains at least `need` primes. */
static void ensure_primes(size_t need)
{
    if (prime_count >= need)
        return;                       /* already have enough */

    /* Get an upper bound for the `need`‑th prime. */
    idx_t limit = _prime_limit_for_idx((idx_t)need);

    /* Rebuild the cache. */
    free(prime_list);
    prime_list = NULL;
    prime_count = 0;
    build_primes_upto(limit, &prime_list, &prime_count);
}

seq_t a(idx_t n)
{
    /* n is 1‑based; undefined for n = 0 */
    if (n == 0)
        return 0;

    /* Need primes p(n) and p(n+1). */
    ensure_primes((size_t)(n + 1));

    uint64_t p_n    = prime_list[(size_t)(n - 1)];
    uint64_t p_next = prime_list[(size_t)(n)];   /* p(n+1) */

    for (idx_t m = 1; ; ++m)
    {
        seq_t val1 = (seq_t)p_n + (seq_t)m * (seq_t)p_next;
        seq_t val2 = (seq_t)m * (seq_t)p_n + (seq_t)p_next;

        if (isPrimeGeneric(val1) && isPrimeGeneric(val2))
            return (seq_t)m;
    }
}
