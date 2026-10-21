/*
 * A083912  Number of divisors of n that are congruent to 2 modulo 10.
 * https://oeis.org/A083912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n that are ≡ 2 (mod 10)
 * Range:     u8 n <= 8677, u16 n <= 8677, u32 n <= 8677, u64 n <= 8677,
 *            u128 n <= 8677, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 1, a(3) = 0, a(4) = 1, … */

#include <stdbool.h>
#include <stdint.h>

/* External types provided by the build system */
extern seq_t a(idx_t n);

/* Computes a(n) as defined above. */
seq_t a(idx_t n)
{
    /* When n is zero, the function is undefined; return 0 for safety. */
    if (n == 0)
        return 0;

    /* Count of divisors congruent to 2 mod 10. */
    seq_t count = 0;

    /* Loop over divisors up to sqrt(n).  Use n/d to avoid overflow. */
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            /* Check d itself. */
            if ((d % 10) == 2)
                ++count;

            /* Check the complementary divisor if distinct. */
            idx_t other = n / d;
            if (other != d && (other % 10) == 2)
                ++count;
        }
    }

    return count;
}
