/*
 * A045446  Primes congruent to {0, 1, 5} mod 7.
 * https://oeis.org/A045446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to {0,1,5} modulo 7. Offset: 1
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* Behaviour below the offset is irrelevant */

    idx_t count = 0;
    seq_t candidate = 5;                     /* first prime in the desired residue classes */

    for (;; ++candidate)
    {
        /* Fast rejection: keep only numbers with residue 0, 1, or 5 modulo 7. */
        int r = (int)(candidate % 7);
        if (r == 0 || r == 1 || r == 5)
        {
            if (isPrimeGeneric(candidate))
            {
                ++count;
                if (count == n)
                    return candidate;
            }
        }
    }
}
