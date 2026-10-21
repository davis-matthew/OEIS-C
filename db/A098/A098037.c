/*
 * A098037  Number of prime divisors, counted with multiplicity, of the sum of
 *          two consecutive primes.
 * https://oeis.org/A098037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A001222(A001043(n))
 * Range:     u8 n <= 30, u16 n <= 3581, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    Number of prime divisors (with multiplicity) of p_n + p_{n+1}  */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto(limit, uint64_t **, size_t *) */
#include "../utils/count_prime_factors.h" /* count_prime_factors(seq_t) */

seq_t a(idx_t n)
{
    /* static cache of primes */
    static uint64_t *primes = NULL;
    static size_t prime_count = 0;
    static idx_t current_limit = 0;

    /* sanity: n>=1 (offset is 1) */
    if (n == 0) {
        return (seq_t)0;
    }

    /* initial limit for the first call */
    if (primes == NULL) {
        current_limit = 200000ULL;          /* enough for n up to about 1e4 */
        build_primes_upto(current_limit, &primes, &prime_count);
    }

    /* enlarge table if necessary */
    while (prime_count < n + 1) {            /* need primes[n] (0‑based) */
        current_limit *= 2;                  /* double the limit */
        free(primes);                        /* free old array */
        build_primes_upto(current_limit, &primes, &prime_count);
    }

    /* p_n and p_{n+1}  (0‑based indexing) */
    uint64_t p_n      = primes[n - 1];
    uint64_t p_next   = primes[n];

    /* sum as seq_t */
    seq_t sum = (seq_t)(p_n + p_next);

    /* return the number of prime factors with multiplicity */
    return count_prime_factors(sum);
}
