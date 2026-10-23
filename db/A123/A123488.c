/*
 * A123488  Smallest prime of the form (q^p-1)/(q-1), where p = prime(n) and q
 *          is also prime (q = A123487(n)).
 * https://oeis.org/A123488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A123487(n)^prime(n) - 1)/(A123487(n) - 1) where
 *            A123487(n) is the smallest prime q such that
 *            (q^prime(n)-1)/(q-1) is prime
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the helper functions:
   - get_prime(idx_t k): returns the k-th prime (1‑based).
   - big_pow(seq_t base, seq_t exp): fast exponentiation.
   - is_prime_big(seq_t x): primality test for arbitrary precision integers. */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Prime exponent p = prime(n) */
    seq_t p = (seq_t)get_prime(n);        /* safe cast: p fits in seq_t */

    /* Check increasing primes q until sum is prime */
    idx_t q_idx = 1;
    seq_t result;

    for (;;)
    {
        uint32_t q_uint = get_prime(q_idx);
        seq_t q = (seq_t)q_uint;          /* convert into seq_t */

        /* Compute s = (q^p - 1)/(q - 1) */
        seq_t power = big_pow(q, p);
        seq_t s = power - (seq_t)1;
        seq_t denom = q - (seq_t)1;
        result = s / denom;              /* exact division */

        if (is_prime_big(result))
            return result;               /* found the required prime */

        ++q_idx;                         /* try next prime q */
    }
}
