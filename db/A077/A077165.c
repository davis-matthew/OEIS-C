/*
 * A077165  Leading terms of rows in A077164.
 * https://oeis.org/A077165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first term of row n of A077164; computed by simulating
 *            the triangle (each row n holds n numbers coprime to n, larger
 *            than all earlier terms)
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-07)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t last = 0;
    for (idx_t row = 1; ; ++row)
    {
        seq_t r = (seq_t)row;
        seq_t m = ((last > r) ? last : r) + 1;
        seq_t first = 0;
        idx_t found = 0;
        while (found < row)
        {
            if (gcd(m, r) == 1)
            {
                if (found == 0)
                    first = m;
                ++found;
            }
            ++m;
        }
        if (row == n)
            return first;
        last = m - 1;
    }
}
