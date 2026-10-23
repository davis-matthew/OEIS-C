/*
 * A039144  Numbers whose base-10 representation has the same number of 4's
 *          and 8's.
 * https://oeis.org/A039144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth nonnegative integer whose decimal representation
 *            contains the same number of 4's and 8's (including 0 which
 *            contains none)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   equal_4_8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/equal_4_8.h"

seq_t a(idx_t n)
{
    seq_t num = 0;
    idx_t found = 0;   /* counts how many qualifying numbers we have seen */

    while (1)
    {
        if (equal_4_8(num))
        {
            found++;
            if (found == n)
                return num;
        }
        num++;   /* next integer to test */
    }
}
