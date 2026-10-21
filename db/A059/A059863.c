/*
 * A059863  a(n) = Product_{i=3..n} (prime(i) - 4).
 * https://oeis.org/A059863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=3..n} (prime(i) – 4) a(1)=a(2)=1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 17, u128 n <= 27,
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
    /* Cache the list of primes up to the largest n received so far. */
    static uint32_t *primes = NULL;
    static size_t primes_cnt = 0;

    if (n < 3)                     /* a(1)=a(2)=1 by definition */
        return 1;

    /* Ensure we have the first n primes available. */
    if ((size_t)n > primes_cnt) {
        if (primes)
            free(primes);          /* Free the old buffer if resizing. */
        sieve_first_n_primes(n, &primes, &primes_cnt);
    }

    /* Compute the product of (prime(i) - 4) for i = 3..n. */
    seq_t result = 1;
    for (idx_t i = 3; i <= n; ++i) {
        result *= (seq_t)(primes[i - 1] - 4);
    }
    return result;
}
