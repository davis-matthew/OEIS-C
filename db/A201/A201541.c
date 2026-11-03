/*
 * A201541  Numbers n such that 12n+5 and 12n+7 are primes.
 * https://oeis.org/A201541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer m such that 12*m+5 and 12*m+7 are prime
 * Range:     u8 n <= 7, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 56 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

/* Compute the n-th integer m for which both 12*m+5 and 12*m+7 are prime. */
seq_t a(idx_t n)
{
    idx_t m = 0;
    idx_t count = 0;            /* number of qualifying m's found so far */

    for (;; m++)
    {
        seq_t p1 = (seq_t)(12 * m + 5);
        seq_t p2 = (seq_t)(12 * m + 7);

        if (is_prime_seq(p1) && is_prime_seq(p2))
        {
            if (count == n)
                return (seq_t)m;
            ++count;
        }
    }
}
