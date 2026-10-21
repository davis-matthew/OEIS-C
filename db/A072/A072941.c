/*
 * A072941  Least multiple of n having no prime gaps.
 * https://oeis.org/A072941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n * ∏p (p∉n) where the product is over all primes p such
 *            that smallest_prime_factor(n) ≤ p ≤ largest_prime_factor(n)
 * Range:     u8 n <= 21, u16 n <= 33, u32 n <= 57, u64 n <= 105,
 *            u128 n <= 205, big any n
 * Helpers:   build_primes_upto, factorize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Utilities used by the function. */
#include "../utils/factorize.h"          /* size_t factorize(uint64_t, uint64_t[], int[]); */
#include "../utils/build_primes_upto.h"  /* void build_primes_upto(idx_t, uint64_t**, size_t*); */

/* Cached list of all primes up to the largest limit requested so far. */
static uint64_t *prime_cache   = NULL;
static size_t   prime_cache_cnt = 0;
static idx_t    prime_cache_limit = 0;

/* Return the a(n) value for the OEIS sequence A072941.  */
seq_t a(idx_t n)
{
    /* Special case n = 1. */
    if (n == 1) return (seq_t)1;

    /* Factor n to find its smallest and largest prime factors. */
    uint64_t primes_fact[64];
    int      exps_fact[64];
    size_t   num_factors = factorize((uint64_t)n, primes_fact, exps_fact);
    if (num_factors == 0)        /* should not happen except for n==1 */
        return (seq_t)1;
    uint64_t spf = primes_fact[0];
    uint64_t lpf = primes_fact[num_factors - 1];

    /* If n is a prime power, all required primes are already present. */
    if (spf == lpf)
        return (seq_t)n;

    /* Ensure we have the list of all primes up to lpf cached. */
    if (lpf > prime_cache_limit) {
        uint64_t *new_primes = NULL;
        size_t    new_cnt    = 0;
        if (prime_cache) free(prime_cache);
        build_primes_upto(lpf, &new_primes, &new_cnt);
        prime_cache        = new_primes;
        prime_cache_cnt    = new_cnt;
        prime_cache_limit  = lpf;
    }

    /* Compute the smallest multiple of n that contains all primes in the range. */
    seq_t result = (seq_t)n;

    /* Find the first index in prime_cache where prime >= spf. */
    size_t i = 0;
    while (i < prime_cache_cnt && prime_cache[i] < spf)
        ++i;

    for (; i < prime_cache_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p > lpf) break;
        if (n % p != 0)         /* p does not divide n – include it */
            result *= (seq_t)p;
    }
    return result;
}
