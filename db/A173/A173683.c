/*
 * A173683  Sum of n mod p, for p = 2, 3, 5, ..., smallest prime >= n.
 * https://oeis.org/A173683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sum of n mod p over all primes p ≤ smallest prime ≥ n
 * Range:     u8 n <= 67, u16 n <= 1567, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h" /* build_primes_upto(uint64_t limit, uint64_t **primes_out, size_t *cnt_out) */
#include "../utils/is_prime.h"        /* is_prime(uint64_t) */

/* Cached list of primes */
static uint64_t *prime_cache = NULL;     /* array of primes up to prime_cache_limit */
static size_t    prime_cache_size = 0;   /* number of primes stored */
static uint64_t  prime_cache_limit = 0;  /* largest prime included in cache */

/* Ensure prime_cache contains all primes ≤ limit. */
static void ensure_prime_cache(uint64_t limit)
{
    if (limit <= prime_cache_limit)
        return;

    /* Rebuild cache up to the new limit. */
    if (prime_cache)
        free(prime_cache);

    build_primes_upto(limit, &prime_cache, &prime_cache_size);
    prime_cache_limit = limit;
}

/* Find the smallest prime ≥ n. (n is ≥ 1 by problem statement.) */
static uint64_t smallest_prime_ge(uint64_t n)
{
    if (n <= 2)
        return 2;
    uint64_t candidate = n;
    while (!is_prime(candidate))
        ++candidate;
    return candidate;
}

seq_t a(idx_t n)
{
    /* n is guaranteed ≥ 1. Convert to uint64_t for prime queries. */
    uint64_t un = (uint64_t)n;

    /* Find the smallest prime ≥ n. */
    uint64_t p_min = smallest_prime_ge(un);

    /* Ensure we have all primes up to p_min. */
    ensure_prime_cache(p_min);

    /* Sum n mod p for all primes p ≤ p_min. */
    seq_t sum = 0;
    for (size_t i = 0; i < prime_cache_size; ++i) {
        uint64_t prime = prime_cache[i];
        if (prime > p_min)
            break;
        seq_t part = (seq_t)un % (seq_t)prime;
        sum += part;
    }
    return sum;
}
