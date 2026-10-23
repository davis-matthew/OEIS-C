/*
 * A154611  Numbers n such that 7*n+3 is not prime.
 * https://oeis.org/A154611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th integer such that 7*n + 3 is not a prime (i.e.,
 *            composite)
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Count the qualifying numbers until the n-th one is found. */
    idx_t count = 0;
    seq_t cand = 1;                     /* candidate integer */
    while (1)
    {
        /* Compute 7*cand + 3 and test primality. */
        seq_t val = 7 * cand + 3;
        if (!isPrimeGeneric(val))
        {
            ++count;
            if (count == n)
                return cand;
        }
        ++cand;
    }
}
