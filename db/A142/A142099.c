/*
 * A142099  Primes congruent to 34 mod 35.
 * https://oeis.org/A142099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 34 mod 35 (computed by a simple search)
 * Range:     u8 n <= 1, u16 n <= 276, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Find the nth prime p such that p % 35 == 34. */
    idx_t k = 1;          /* k in the expression 35*k + 34 */
    idx_t found = 0;      /* number of primes found so far */
    seq_t candidate;

    while (1)
    {
        /* Compute the next number of the form 35*k + 34 */
        candidate = (seq_t)35 * (seq_t)k + 34;

        /* Test primality. is_prime expects an idx_t argument. */
        if (is_prime((idx_t)candidate))
        {
            found++;
            if (found == n)
                return candidate;
        }
        k++;
    }
}
