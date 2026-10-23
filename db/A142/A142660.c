/*
 * A142660  Primes congruent to 45 mod 56.
 * https://oeis.org/A142660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime congruent to 45 mod 56
 * Range:     u8 n <= 2, u16 n <= 276, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* n is 1‑based.  The first prime of the form 56*k+45 is 101 (k=1). */
    idx_t count = 0;
    idx_t k = 1;
    seq_t result = 0;

    while (count < n)
    {
        /* Candidate prime */
        seq_t cand = (seq_t)56 * (seq_t)k + (seq_t)45;

        /* Test primality */
        if (is_prime((idx_t)cand))
        {
            count++;
            if (count == n)
            {
                result = cand;
                break;
            }
        }
        k++;
    }
    return result;
}
