/*
 * A106091  Primes p such that 7*p + 12 and 12*p + 7 are primes.
 * https://oeis.org/A106091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes p such that 7p+12 and 12p+7 are primes
 * Range:     u8 n <= 19, u16 n <= 642, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* undefined offset – safe return */

    idx_t found = 0;
    uint64_t cand = 2;                  /* first possible prime */

    while (1) {
        if (is_prime_uint64(cand)) {    /* cand is prime */
            /* check the two linear combinations */
            if (is_prime_uint64(7ULL * cand + 12ULL) &&
                is_prime_uint64(12ULL * cand + 7ULL))
            {
                ++found;
                if (found == n)
                    return (seq_t)cand; /* term found */
            }
        }

        /* advance to the next candidate:
           after 2, only odd numbers need be tested      */
        cand += (cand == 2) ? 1 : 2;
    }
}
