/*
 * A045458  Primes congruent to 5 mod 7.
 * https://oeis.org/A045458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime congruent to 5 modulo 14
 * Range:     u8 n <= 9, u16 n <= 856, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined offset */
        return 0;

    idx_t count = 0;
    seq_t candidate = 5;   /* first prime of the form 14k+5 */

    while (count < n)
    {
        if (is_prime_big(candidate))
        {
            ++count;
            if (count == n)
                return candidate;
        }
        candidate += 14;   /* next integer ≡ 5 (mod 14) */
    }

    return 0;   /* unreachable, keeps compiler happy */
}
