/*
 * A071555  Smallest x > 1 such that x^prime(n) == 1 mod(prime(i)) 3<=i<=n.
 * https://oeis.org/A071555
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + primorial(n)/6, where primorial(n) is the product of
 *            the first n primes
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 3; for smaller n the return value is irrelevant. */
    if (n < 3) return 0;

    /* Cache the list of the first n primes so repeated calls are fast. */
    static uint32_t *prime_list = NULL;
    static size_t prime_list_cnt = 0;

    if (prime_list_cnt < n) {
        /* Rebuild the array if we need more primes. */
        if (prime_list) free(prime_list);
        sieve_first_n_primes(n, &prime_list, &prime_list_cnt);
    }

    /* Compute primorial(n)/6 = product of primes[2]..primes[n-1] (skipping 2 and 3). */
    seq_t rest = 1;
    for (size_t i = 2; i < (size_t)n; ++i) {
        rest *= (seq_t)prime_list[i];
    }

    /* a(n) = 1 + primorial(n)/6 */
    return 1 + rest;
}
