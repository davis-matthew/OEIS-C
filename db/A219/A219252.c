/*
 * A219252  Smallest prime q such that 2*n+1 = p + 4*q for some odd prime p,
 *          otherwise 0 if no such q exists.
 * https://oeis.org/A219252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime q such that 2*n+1 = p + 4*q for some odd
 *            prime p, otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a static cache of primes up to the largest q
   needed so far.  It relies on the utility functions
   build_primes_upto() and is_prime() defined in the utils library. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* target = 2*n+1 */
    uint64_t target = (uint64_t)n * 2ULL + 1ULL;

    /* Largest q that can satisfy 2*n+1 = p + 4*q is floor((target-3)/4). */
    uint64_t max_q = (target > 3ULL) ? (target - 3ULL) / 4ULL : 0ULL;

    if (max_q < 2ULL)          /* no prime q ≥ 2 is possible */
        return 0;

    /* Static cache of primes up to the maximum q needed */
    static uint64_t *prime_cache = NULL;
    static size_t   prime_cnt   = 0;
    static idx_t    prime_limit = 0;

    /* Rebuild the cache if the current limit is too small */
    if (prime_limit < max_q) {
        free(prime_cache);
        build_primes_upto(max_q, &prime_cache, &prime_cnt);
        prime_limit = max_q;
    }

    /* Search primes q in increasing order */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = prime_cache[i];
        if (q > max_q)
            break;
        uint64_t p = target - 4ULL * q;
        if (p < 3ULL)                         /* p must be odd prime ≥3 */
            continue;
        if ((p & 1ULL) && is_prime((idx_t)p)) /* check primality of p */
            return (seq_t)q;
    }

    return 0; /* no suitable q found */
}
