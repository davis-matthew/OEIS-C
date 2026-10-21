/*
 * A054972  Product of (sum of first n primes) and (product of first n
 *          primes).
 * https://oeis.org/A054972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (sum of first n primes) * (product of first n primes)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1, but guard against 0 */

    /* Cache a list of the first n primes */
    static uint32_t *primes = NULL;
    static size_t primes_cnt = 0;

    /* If we don't yet have enough primes, reallocate */
    if ((idx_t)primes_cnt < n)
    {
        if (primes) free(primes);
        sieve_first_n_primes(n, &primes, &primes_cnt);
    }

    seq_t sum = 0;
    seq_t prod = 1;

    for (size_t i = 0; i < (size_t)n; ++i)
    {
        seq_t p = (seq_t)primes[i];
        sum += p;
        prod *= p;
    }

    return sum * prod;
}
