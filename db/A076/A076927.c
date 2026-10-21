/*
 * A076927  a(n) = A076926(n)/n.
 * https://oeis.org/A076927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator of n * sum_{k=1}^n 1/prime(k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   build_primes_upto, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent integer formula: a(n) = product of the first n primes that do NOT divide n */

#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Cached list of the first primes as needed */
    static uint64_t *prime_list = NULL;
    static size_t prime_count = 0;
    static idx_t cached_n = 0;

    /* Ensure we have the first n primes in prime_list */
    if (n > cached_n) {
        free(prime_list);               /* release old buffer */
        prime_list   = NULL;
        prime_count  = 0;
        cached_n     = n;

        /* The n-th prime is the upper bound for which the first n primes lie */
        idx_t limit = get_prime(n);

        /* Build all primes up to that limit */
        build_primes_upto(limit, &prime_list, &prime_count);
        /* prime_count should now be equal to n */
    }

    /* Compute product of primes that do NOT divide n */
    seq_t prod = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        uint64_t p = prime_list[i];
        if (n % p != 0) {
            prod *= (seq_t)p;
        }
    }
    return prod;
}
