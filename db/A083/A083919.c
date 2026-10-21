/*
 * A083919  Number of divisors of n that are congruent to 9 modulo 10.
 * https://oeis.org/A083919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ d | n : d ≡ 9 (mod 10) }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of divisors of n that are congruent to 9 modulo 10. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is >= 1 */
    uint64_t nn = (uint64_t)n;
    uint64_t count = 0;

    for (uint64_t d = 1; d * d <= nn; ++d)
    {
        if (nn % d == 0)
        {
            if (d % 10 == 9) ++count;
            uint64_t other = nn / d;
            if (other != d && other % 10 == 9) ++count;
        }
    }

    return (seq_t)count;
}
