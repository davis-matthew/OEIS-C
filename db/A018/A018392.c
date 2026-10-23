/*
 * A018392  Divisors of 322.
 * https://oeis.org/A018392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 322 in ascending order
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If n is out of range (n < 1 or n > 8) the function returns 0. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    const uint64_t target = 322ULL;
    idx_t d = 0;
    idx_t found = 0;

    for (d = 1; d <= target; ++d)
    {
        if (target % d == 0)
        {
            ++found;
            if (found == n)
                return (seq_t)d;
        }
    }

    return 0;  /* n is larger than the number of divisors of 322 */
}
