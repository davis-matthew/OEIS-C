/*
 * A062022  a(n) = Sum_{k=1..n} Sum_{j=1..k} (prime(k) - prime(j))^2.
 * https://oeis.org/A062022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*Sum_{k=1..n} prime(k)^2 - (Sum_{k=1..n} prime(k))^2
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 190, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalent to the double sum of squared differences of the first n primes) */

#include <stdint.h>
#include <stdlib.h>
/* The utility provides the sieve for the first n primes */
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    /* Allocate array for the first n primes */
    uint32_t *primes = NULL;
    size_t cnt = 0;
    sieve_first_n_primes(n, &primes, &cnt);   /* primes[0..n-1] hold the first n primes */

    seq_t sumP = 0;
    seq_t sumSq = 0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t p = (seq_t)primes[i];
        sumP   += p;
        sumSq  += p * p;
    }

    /* Free the temporary prime array */
    free(primes);

    seq_t result = (seq_t)n * sumSq - sumP * sumP;
    return result;
}
