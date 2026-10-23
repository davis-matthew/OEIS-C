/*
 * A179816  Sum of the prime numbers that are between 10*n and 10*(n+1).
 * https://oeis.org/A179816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1803, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*****************************************************
 *  Sequence A179816
 *  Formula: a(n) = sum of all prime numbers p such that
 *                10*n ≤ p ≤ 10*(n+1).
 *  Implementation: On first use or when a larger upper bound
 *  is required, a sieve of Eratosthenes generates all primes
 *  up to 10*(n+1); thereafter the prime list is reused.
 *****************************************************/

#include <stdlib.h>
#include "../utils/build_primes.h"

seq_t a(idx_t n) {
    /* static state: cached prime list and current limit */
    static idx_t max_limit = 0;          /* upper bound previously sieved */
    static idx_t *prime_list = NULL;     /* array of primes, type idx_t   */
    static size_t prime_count = 0;       /* number of stored primes       */

    /* Compute bounds for this term */
    const idx_t lower = 10 * n;
    const idx_t upper = 10 * (n + 1);

    /* Extend prime sieve if needed */
    if (upper > max_limit) {
        if (prime_list) free(prime_list);          /* free old array */
        build_primes(upper, &prime_list, &prime_count);
        max_limit = upper;
    }

    /* Sum primes in [lower, upper] */
    seq_t sum = 0;

    /* Skip primes below lower */
    size_t i = 0;
    for (i = 0; i < prime_count && prime_list[i] < lower; ++i)
        ;
    /* Add primes up to upper */
    for (; i < prime_count; ++i) {
        if (prime_list[i] > upper) break;
        sum += (seq_t)prime_list[i];
    }

    return sum;
}
