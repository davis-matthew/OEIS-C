/*
 * A215212  Primes congruent to {2, 5, 7} mod 17.
 * https://oeis.org/A215212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 999, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 2, 5, or 7 modulo 17.
   The function returns the n‑th such prime (1‑based indexing). */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n < 1 */

    idx_t found = 0;              /* number of qualifying primes seen */
    uint64_t x = 2;               /* current integer to test */

    while (1)
    {
        /* Check the congruence condition */
        if (x % 17 == 2 || x % 17 == 5 || x % 17 == 7)
        {
            /* Test primality of x using the provided helper */
            if (is_prime_big((seq_t)x))
            {
                ++found;
                if (found == n)
                    return (seq_t)x;
            }
        }
        ++x;
    }
}
