/*
 * A076873  Smallest prime not less than sum of first n primes.
 * https://oeis.org/A076873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 155, u32 n <= 48, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest prime not less than sum of first n primes.
   a(n) defined as the smallest prime >= sum_{k=1}^{n} p_k,
   where p_k is the k-th prime. */
#include <stdlib.h>
#include <stdint.h>

#include "../utils/is_prime_big.h"
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    /* Cache the first n primes to avoid recomputation. */
    static idx_t cached_n = 0;
    static uint32_t *primes = NULL;
    static size_t primes_len = 0;

    if (n > (idx_t)cached_n) {
        /* Resize cache to accommodate the new n. */
        free(primes);                /* free previous cache, if any */
        sieve_first_n_primes(n, &primes, &primes_len);
        cached_n = n;
    }

    /* Compute the sum of the first n primes in seq_t space. */
    seq_t sum = 0;
    for (idx_t i = 0; i < n; ++i) {
        sum += (seq_t)primes[i];
    }

    /* Find the smallest prime not less than sum. */
    seq_t candidate = sum;
    while (!is_prime_big(candidate)) {
        ++candidate;
    }
    return candidate;
}
