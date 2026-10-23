/*
 * A142113  Primes congruent to 4 mod 37.
 * https://oeis.org/A142113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 196, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime that is congruent to 4 modulo 37
   by searching numbers of the form 37*k + 4 and testing primality. */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Offset is 1; n=0 is undefined, return 0 */

    idx_t count = 0;        /* How many such primes we have found */
    idx_t k = 1;            /* Start with the first candidate: 37*1+4 = 41 */

    while (count < n)
    {
        idx_t candidate = 37 * k + 4;   /* Candidate number */
        if (is_prime(candidate))
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;   /* Return the nth prime */
        }
        ++k;
    }

    /* Unreachable, but keep compiler happy */
    return 0;
}
