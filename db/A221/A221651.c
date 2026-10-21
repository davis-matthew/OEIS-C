/*
 * A221651  Numbers divisible by their first digit squared (excluding those
 *          whose first digit is 1).
 * https://oeis.org/A221651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose first digit d satisfies
 *            d^2 divides the integer (d > 1)
 * Range:     u8 n <= 20, u16 n <= 5392, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   first_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/first_digit.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t found = 0;
    for (idx_t val = 2; ; ++val)
    {
        idx_t d = first_digit(val);
        if (d <= 1)
            continue;
        seq_t d2 = (seq_t)d * d;
        seq_t vi = (seq_t)val;
        if (vi % d2 == 0)
        {
            ++found;
            if (found == n)
                return vi;
        }
    }
}
