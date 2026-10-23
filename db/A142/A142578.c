/*
 * A142578  Primes congruent to 48 mod 53.
 * https://oeis.org/A142578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p ≡ 48 (mod 53). Offset = 1
 * Range:     u8 n <= 1, u16 n <= 108, u32 n <= 124, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n = 0, return 0 as a safe fallback */

    idx_t found = 0;                /* number of primes of the form found so far */
    idx_t k     = 1;                /* index in the arithmetic progression 48 + 53*k */

    while (true)
    {
        seq_t candidate = (seq_t)48 + (seq_t)53 * (seq_t)k;

        if (is_prime_big(candidate))
        {
            ++found;
            if (found == n)
                return candidate;   /* return the nth prime */
        }

        ++k;
    }
}
