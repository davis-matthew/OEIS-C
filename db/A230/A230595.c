/*
 * A230595  Number of ways to write n as n = x*y, where x and y are primes, 1
 *          <= x <= n, 1 <= y <= n.
 * https://oeis.org/A230595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A010051(d) * A010051(n/d) (Dirichlet *
 *            convolution of the characteristic function of primes with
 *            itself)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

/* Cached prime table for fast factorisation */
static uint64_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;
static idx_t prime_cache_limit = 0;

/* Ensure that prime_cache contains all primes <= limit.   */
static void ensure_primes(idx_t limit)
{
    if (prime_cache != NULL && prime_cache_limit >= limit)
        return;

    if (prime_cache != NULL)
        free(prime_cache);

    build_primes_upto(limit, &prime_cache, &prime_cache_cnt);
    prime_cache_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 4)                 /* 1, 2, 3 are not semiprimes */
        return (seq_t)0;

    ensure_primes(n);

    idx_t i;
    uint64_t p = 0, q;
    for (i = 0; i < prime_cache_cnt; ++i) {
        p = prime_cache[i];
        if (p * (idx_t)p > n)      /* p^2 > n */
            break;
        if (n % p == 0) {
            q = n / p;
            if (is_prime(q)) {
                /* Check whether we have exactly two prime factors */
                if (p == (uint64_t)q)
                    return (seq_t)1;   /* p^2 */
                else
                    return (seq_t)2;   /* p * q with p != q */
            }
            /* If q is not prime, n has more than 2 prime factors or is a
             * power of a single prime higher than 2, so a(n) = 0. */
            return (seq_t)0;
        }
    }
    /* No prime factor found up to sqrt(n) -> n is prime -> a(n) = 0 */
    return (seq_t)0;
}
