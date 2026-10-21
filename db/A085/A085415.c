/*
 * A085415  Take prime(n) and continue adding 1, 2, ..., a(n) until one
 *          reaches a prime.
 * https://oeis.org/A085415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest m > 0 such that * get_prime(n) + m*(m+1)/2 is
 *            prime
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

/* Computes a(n) for the sequence A085415.  */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1, but the function
       tolerates n == 0 by returning 0. */
    if (n == 0)
        return (seq_t)0;

    /* Obtain the n‑th prime.  get_prime returns a 32‑bit
       unsigned integer, which is safely cast into seq_t. */
    seq_t prime = (seq_t)get_prime(n);

    /* Search for the minimal positive m such that
       prime + T_m is prime, where T_m = m*(m+1)/2.  */
    for (idx_t m = 1; ; ++m)
    {
        seq_t t   = (seq_t)m * (seq_t)(m + 1) / 2;   /* T_m */
        seq_t s   = prime + t;                       /* candidate */
        if (is_prime_big(s))
            return (seq_t)m;
    }
}
