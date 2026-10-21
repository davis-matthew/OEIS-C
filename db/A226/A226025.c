/*
 * A226025  Odd composite numbers that are not squares of primes.
 * https://oeis.org/A226025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = odd composite number that is not a square of a prime
 *            (OEIS A226025)
 * Range:     u8 n <= 11, u16 n <= 6048, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/floor_sqrt.h"

/* Return the n-th odd composite number that is not a square of a prime.
   For n < 1 this function returns 0. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;

    idx_t    count = 0;      /* number of terms found so far */
    seq_t    cand  = (seq_t)3; /* first odd number > 1 */

    for (;;)
    {
        /* candidate is composite if it is not prime */
        if (!isPrimeGeneric(cand))
        {
            /* reject if cand is the square of a prime */
            seq_t r = floor_sqrt(cand);
            if (!(r * r == cand && isPrimeGeneric(r)))
            {
                ++count;
                if ((idx_t)count == n)
                    return cand;
            }
        }
        cand += (seq_t)2;     /* next odd number */
    }
}
