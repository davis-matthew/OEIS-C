/*
 * A077166  Final terms of rows in A077164.
 * https://oeis.org/A077166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = last term of row n of A077164, where row r lists r
 *            numbers coprime to r, each greater than all earlier terms;
 *            computed by direct search
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-07)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t last = 0;
    for (idx_t row = 1; row <= n; ++row)
    {
        seq_t r = (seq_t)row;
        seq_t m = (last > r) ? last : r;
        idx_t found = 0;
        while (found < row)
        {
            ++m;
            if (gcd(m, r) == 1)
                ++found;
        }
        last = m;
    }
    return last;
}
