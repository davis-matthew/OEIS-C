/*
 * A142306  Primes congruent to 31 mod 44.
 * https://oeis.org/A142306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 31 modulo 44 (a(1)=31)
 * Range:     u8 n <= 3, u16 n <= 334, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined at index 0, safe fallback */

    idx_t found = 0;               /* how many primes of the form 44k+31 seen */

    for (idx_t k = 0;; ++k)        /* iterate over all k ≥ 0 */
    {
        idx_t val = 44 * k + 31;   /* candidate number */

        if (is_prime(val))        /* check primality */
        {
            ++found;
            if (found == n)       /* n-th such prime reached */
                return (seq_t)val;
        }
    }
}
