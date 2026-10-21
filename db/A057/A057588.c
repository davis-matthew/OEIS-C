/*
 * A057588  Kummer numbers: -1 + product of first n consecutive primes.
 * https://oeis.org/A057588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (product of the first n primes) - 1 (OEIS A057588)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    /* Default to 0 for safety when n < 1, although the sequence is defined for n >= 1 */
    if (n < 1) return 0;

    uint32_t *primes = NULL;
    size_t primecnt = 0;

    /* Obtain the first n primes.  sieve_first_n_primes returns uint32_t pointers,
       which is sufficient because the nth prime for n ≤ 10000 is 104729. */
    sieve_first_n_primes(n, &primes, &primecnt);

    /* Compute the product and subtract one. */
    seq_t prod = 1;
    for (size_t i = 0; i < primecnt; ++i) {
        prod *= (seq_t)primes[i];
    }

    free(primes);
    return prod - 1;
}
