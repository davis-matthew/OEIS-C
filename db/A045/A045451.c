/*
 * A045451  Primes congruent to {1, 4, 5} mod 7.
 * https://oeis.org/A045451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to {1, 4, 5} modulo 7
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n=0, harmless */

    idx_t count    = 0;                  /* how many primes found so far */
    idx_t candidate = 5;                 /* first prime with desired residue */

    for (;;)
    {
        /* test primality and residue modulo 7 */
        if (is_prime(candidate) &&
            (candidate % 7 == 1 || candidate % 7 == 4 || candidate % 7 == 5))
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }

        /* advance to next odd number */
        candidate += 2;
    }
}
