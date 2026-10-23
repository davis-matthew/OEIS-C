/*
 * A133534  Sum of third powers of two consecutive primes.
 * https://oeis.org/A133534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^3 + prime(n+1)^3 (OEIS A133534)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 209, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes.h"   /* build_primes */

seq_t a(idx_t n)
{
    /* Return 0 for non‑positive indices (undefined by OEIS). */
    if (n == 0) return 0;

    /* Static cache of generated primes. */
    static idx_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t primes_limit = 0;

    /* Ensure we have at least (n+1) primes in the cache. */
    if (primes_cnt <= (size_t)n + 1) {
        /* Rough upper bound for the (n+1)-th prime.
           For n up to 10k this is far more than enough. */
        idx_t required = (idx_t)n + 1;
        idx_t limit = required * 20 + 1000;

        /* If we already have a larger limit, reuse it. */
        if (limit < primes_limit) limit = primes_limit;

        /* Regenerate primes up to the new limit. */
        if (primes) free(primes);
        build_primes(limit, &primes, &primes_cnt);
        primes_limit = limit;

        /* If the limit was still too small, double it until enough. */
        while (primes_cnt <= (size_t)required) {
            idx_t new_limit = limit * 2;
            if (primes) free(primes);
            build_primes(new_limit, &primes, &primes_cnt);
            primes_limit = new_limit;
            limit = new_limit;
        }
    }

    /* Retrieve the n‑th and (n+1)‑th primes (array is 0‑based). */
    idx_t p1 = primes[n - 1];
    idx_t p2 = primes[n];

    /* Compute the sum of their cubes using seq_t arithmetic. */
    seq_t s1 = (seq_t)p1;
    seq_t s2 = (seq_t)p2;
    seq_t cube1 = s1 * s1 * s1;
    seq_t cube2 = s2 * s2 * s2;
    return cube1 + cube2;
}
