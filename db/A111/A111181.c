/*
 * A111181  a(n) = prime(n) - pi(n).
 * https://oeis.org/A111181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) - pi(n) (pi(n) = #primes ≤ n)
 * Range:     u8 n <= 58, u16 n <= 6621, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/estimate_prime_upper_bound.h"

/* Return the nth term of OEIS sequence A111181.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n<1, safe guard */

    /* Static cache for primes and prime counts up to the current max.      */
    static uint64_t *prime_list      = NULL; /* nth prime at prime_list[n-1]   */
    static size_t   prime_count      = 0;
    static uint32_t *pi_table        = NULL; /* pi_table[i] = #primes ≤ i    */
    static size_t   pi_size          = 0;    /* size of pi_table (max i + 1) */
    static uint64_t max_needed_n     = 0;    /* largest n processed so far   */

    /* Expand the cache if a larger n is requested.                         */
    if (n > max_needed_n) {
        max_needed_n = n;
        /* Estimate upper bound for the nth prime.                         */
        uint64_t upper = estimate_prime_upper_bound(max_needed_n);
        /* We also need a table up to at least n for pi(n).                 */
        uint64_t limit = (upper > (uint64_t)n) ? upper : (uint64_t)n;

        /* Allocate temporary sieve array.                                  */
        uint8_t *sieve = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
        if (!sieve)
            abort();
        for (uint64_t i = 0; i <= limit; ++i)
            sieve[i] = 1;
        sieve[0] = sieve[1] = 0;

        /* Classic sieve of Eratosthenes.                                   */
        for (uint64_t p = 2; p * p <= limit; ++p)
            if (sieve[p])
                for (uint64_t j = p * p; j <= limit; j += p)
                    sieve[j] = 0;

        /* Count primes and fill pi_table.                                  */
        size_t count = 0;
        for (uint64_t i = 2; i <= limit; ++i)
            if (sieve[i])
                ++count;

        /* Reallocate prime list to fit all primes.                          */
        uint64_t *new_prime_list = (uint64_t *)realloc(prime_list,
                                                       count * sizeof(uint64_t));
        if (!new_prime_list)
            abort();
        prime_list = new_prime_list;
        prime_count = count;

        /* Reallocate pi_table.                                            */
        uint32_t *new_pi_table = (uint32_t *)realloc(pi_table,
                                                     (limit + 1) * sizeof(uint32_t));
        if (!new_pi_table)
            abort();
        pi_table = new_pi_table;
        pi_size = limit + 1;

        /* Fill prime list and pi_table in one pass.                        */
        size_t idx = 0;
        uint32_t accum = 0;
        for (uint64_t i = 0; i <= limit; ++i) {
            if (i >= 2 && sieve[i]) {
                pi_table[i] = ++accum;
                prime_list[idx++] = i;
            } else {
                pi_table[i] = accum;
            }
        }

        free(sieve);
    }

    /* Retrieve nth prime and pi(n).                                          */
    if (n > prime_count)
        return (seq_t)0; /* should not happen, safeguard */

    seq_t prime_n = (seq_t)prime_list[n - 1];
    seq_t pi_n    = (seq_t)pi_table[n];

    return prime_n - pi_n;
}
