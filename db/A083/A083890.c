/*
 * A083890  Number of divisors of n with largest digit = 3 (base 10).
 * https://oeis.org/A083890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors of n whose decimal representation has
 *            largest digit equal to 3 (i.e., all digits ≤3 and at least one
 *            3)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_largest_digit_three
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_largest_digit_three.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0)
        return count;           /* defensive; n ≥ 1 by definition */

    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            if (has_largest_digit_three(d))
                ++count;
            idx_t other = n / d;
            if (other != d && has_largest_digit_three(other))
                ++count;
        }
    }
    return count;
}
