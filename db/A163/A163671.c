/*
 * A163671  Expansion of Sum_{k>=1} x^k / (1 - x^(k^2)).
 * https://oeis.org/A163671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that (n - d) is divisible
 *            by d^2 i.e. n = d * (d * t + 1) for some integer t
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for the OEIS sequence A163671. */
#include <stdint.h>
#include <stdbool.h>

/* In the build, seq_t and idx_t are predefined integer types. */

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* Enumerate divisors d of n using a standard O(sqrt(n)) loop. */
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0) continue;          /* d is not a divisor */

        /* Test the divisor d. */
        if ((n - d) % (d * d) == 0)
            ++count;

        /* If d and n/d are distinct, test the complementary divisor. */
        idx_t q = n / d;
        if (q != d)
        {
            if ((n - q) % (q * q) == 0)
                ++count;
        }
    }
    return count;
}
