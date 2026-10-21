/*
 * A209929  Smallest digit of all divisors of n.
 * https://oeis.org/A209929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = smallest digit among all digits of all divisors of n
 * Range:     u8 n <= 9018, u16 n <= 9018, u32 n <= 9018, u64 n <= 9018,
 *            u128 n <= 9018, big any n
 * Helpers:   _smallest_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_digit.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* behaviour below offset does not matter */
        return 0;

    uint64_t nn = (uint64_t)n;
    seq_t best = 9;

    for (uint64_t i = 1; i * i <= nn; ++i)
    {
        if (nn % i == 0)
        {
            seq_t md = _smallest_digit(i);
            if (md < best)
            {
                best = md;
                if (best == 0)
                    return 0;
            }

            uint64_t j = nn / i;
            if (j != i)
            {
                md = _smallest_digit(j);
                if (md < best)
                {
                    best = md;
                    if (best == 0)
                        return 0;
                }
            }
        }
    }
    return best;
}
