/*
 * A073649  Define the composite field of a prime q to be f(q) = (t,s) if p,
 *          q, r are three consecutive primes and q-p = t and r-q = s. This is
 *          a sequence of primes q with field (4,2).
 * https://oeis.org/A073649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime q such that q-4 and q+2 are primes and q-2
 *            is not prime, i.e. q is the middle of a triple of consecutive
 *            primes (p, q, r) with gaps (4, 2)
 * Range:     u8 n <= 8, u16 n <= 185, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function searches primes sequentially using the provided
   Miller‑Rabin primality test (is_prime) and next_prime
   generator. */

#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t q = 2;                 /* start from 2 */
    while (1)
    {
        q = next_prime(q + 1);      /* next odd prime after current q */
        if (q < 5)                  /* q must be at least 5 to have q-4 >= 1 */
            continue;
        /* Check the definition: q-4 prime, q-2 not prime, q+2 prime */
        if (is_prime(q - 4) &&
            !is_prime(q - 2) &&
            is_prime(q + 2))
        {
            count++;
            if (count == n)
                return (seq_t)q;
        }
    }
}
