/*
 * A208936  Prime production length of the polynomial P = x^2 + x + prime(n):
 *          max { k>0 | P(x) is prime for all x=0,...,k-1 }.
 * https://oeis.org/A208936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6995, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 101 terms through n = 9678 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the length of prime production of P(x)=x^2+x+prime(n)
   by brute‑force search.  Uses a sieve to obtain primes up to the n‑th
   prime and a generic primality test. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
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

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

seq_t a(idx_t n)
{
    /* Cache of the first N primes for reuse across calls. */
    static uint64_t *prime_list = NULL;
    static size_t      prime_cnt  = 0;
    static idx_t       max_n_cached = 0;   /* largest n for which the cache is valid */

    /* Rebuild cache if we need more primes. */
    if (prime_list == NULL || n > max_n_cached) {
        idx_t limit = _prime_limit_for_idx(n);
        free(prime_list);                     /* discard old array */
        build_primes_upto(limit, &prime_list, &prime_cnt);
        max_n_cached = n;
    }

    /* The prime used in the polynomial is the n‑th prime (1‑based). */
    seq_t p = (seq_t)prime_list[n - 1];

    /* Count consecutive primes of P(x)=x^2+x+p. */
    seq_t count = 0;
    for (idx_t x = 0; ; ++x) {
        seq_t val = ((seq_t)x * x) + x + p;
        if (!isPrimeGeneric(val))
            break;
        ++count;
    }
    return count;
}
