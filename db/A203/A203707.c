/*
 * A203707  Vandermonde sequence using x^2 + y^2 applied to the first n
 *          primes.
 * https://oeis.org/A203707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{1 <= i < j <= n} (p_i^2 + p_j^2) where p_k is
 *            the k-th prime
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
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
    if (n < 1)
        return (seq_t)0;

    if (n <= 1)
        return (seq_t)1;

    uint32_t *primes = NULL;
    size_t primes_cnt = 0;
    sieve_first_n_primes(n, &primes, &primes_cnt);

    seq_t prod = (seq_t)1;

    for (idx_t i = 0; i + 1 < n; ++i)
    {
        uint32_t pi = primes[i];
        seq_t pi_sq = (seq_t)pi * (seq_t)pi;
        for (idx_t j = i + 1; j < n; ++j)
        {
            uint32_t pj = primes[j];
            seq_t val = pi_sq + (seq_t)pj * (seq_t)pj;
            prod *= val;
        }
    }

    free(primes);
    return prod;
}
