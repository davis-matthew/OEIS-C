/*
 * A039167  Numbers whose base-11 representation has the same number of 1's
 *          and 4's.
 * https://oeis.org/A039167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th nonnegative integer whose base‑11
 *            representation contains an equal number of digits 1 and 4
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   has_equal_1_4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/has_equal_1_4.h"

seq_t a(idx_t n)
{
    seq_t x = 0;
    idx_t found = 0;
    while (1)
    {
        if (has_equal_1_4(x))
        {
            ++found;
            if (found == n)
                return x;
        }
        ++x;
    }
}
