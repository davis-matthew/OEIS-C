/*
 * A172989  Smallest k such that the two numbers n^2 +- k are primes.
 * https://oeis.org/A172989
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > 0 such that n^2 - k and n^2 + k are prime
 * Range:     u8 n <= 15, u16 n <= 706, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* n is assumed to be >= 2, the sequence is defined only for those values. */
    seq_t sq = (seq_t)n * (seq_t)n;   /* compute n^2 in the unsigned type seq_t */

    for (seq_t k = 1; k < sq; ++k)
    {
        seq_t low  = sq - k;
        seq_t high = sq + k;

        if (isPrimeGeneric(low) && isPrimeGeneric(high))
            return k;                 /* smallest k found */
    }

    /* In theory this point is unreachable as a solution must exist. */
    return 0;
}
