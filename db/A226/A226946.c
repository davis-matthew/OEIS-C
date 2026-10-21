/*
 * A226946  Numbers that can't be written as x^2 + x*y + y^2, with 0 <= x <= y
 *          and gcd(x,y) = 1.
 * https://oeis.org/A226946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer ≥ 2 that is either divisible by 9 or has
 *            a prime divisor congruent to 2 modulo 3. This is equivalent to
 *            “numbers that cannot be written as x^2 + x*y + y^2 with 0 ≤ x ≤
 *            y and gcd(x, y) = 1”
 * Range:     u8 n <= 211, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_bad_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_bad_factor.h"

seq_t a(idx_t n)
{
    idx_t count = 0u;
    idx_t m = 2u;                     /* start at 2 (offset 1) */

    while (count < n)
    {
        if (has_bad_factor(m))
        {
            ++count;
            if (count == n)
                return (seq_t)m;      /* found the n-th term */
        }
        ++m;
    }
    return (seq_t)0;                  /* never reached */
}
