/*
 * A048243  Number of non-unitary divisors of binomial(n, floor(n/2)).
 * https://oeis.org/A048243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non-unitary divisors of binomial(n, floor(n/2))
 * Range:     u8 n <= 27, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   build_primes_upto, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the helper functions:
 *   build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
 *   pow2(idx_t exp) -> seq_t  (returns 2^exp)
 *
 * It is memoised with a static cache of primes up to the largest n requested.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow2.h"

static uint64_t *prime_cache = NULL;   /* array of primes up to current limit */
static size_t prime_cache_cnt = 0;     /* number of primes in cache */
static idx_t prime_cache_limit = 0;   /* maximum n processed so far */

/* Ensure that prime_cache holds all primes <= limit (n). */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_cache_limit) return;  /* already covered */
    if (prime_cache) free(prime_cache);       /* free old cache */
    build_primes_upto(limit, &prime_cache, &prime_cache_cnt);
    prime_cache_limit = limit;
}

/* Count exponent of prime p in n! using Legendre's formula. */
static idx_t fact_prime_exponent(idx_t n, uint64_t p)
{
    idx_t exp = 0;
    while (n) {
        n /= p;
        exp += n;
    }
    return exp;
}

seq_t a(idx_t n)
{
    /* base case n = 0 is not used (offset is 1), but handle it safely */
    if (n == 0) return 0;

    idx_t k = n >> 1;          /* floor(n/2) */
    idx_t nk = n - k;          /* other half */

    /* primes up to n are required */
    ensure_primes(n);

    seq_t tau = (seq_t)1;      /* divisor count */
    size_t omega = 0;          /* number of distinct prime factors */

    for (size_t i = 0; i < prime_cache_cnt && prime_cache[i] <= n; ++i) {
        uint64_t p = prime_cache[i];

        /* exponent in n!, k!, and (n-k)! */
        idx_t e = fact_prime_exponent(n, p)
                - fact_prime_exponent(k, p)
                - fact_prime_exponent(nk, p);

        if (e > 0) {
            tau *= (seq_t)(e + (seq_t)1);  /* τ *= (e+1) */
            ++omega;                      /* distinct prime found */
        }
    }

    /* unitary divisor count = 2^omega */
    seq_t unitary = pow2((idx_t)omega);
    return tau - unitary;
}
