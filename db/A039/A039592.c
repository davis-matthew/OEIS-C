/*
 * A039592  Numbers whose base-6 representation has the same number of 3's,
 *          4's and 5's.
 * https://oeis.org/A039592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   has_equal_345
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the nth non‑negative integer (n ≥ 1, offset 1)
   whose base‑6 representation contains an equal number of the digits
   3, 4 and 5.  The first few terms are 0, 1, 2, 6, 7, 8, 12, 13, 14, … */

#include <stdbool.h>
#include "../utils/has_equal_345.h"

seq_t a(idx_t n)
{
    idx_t found = (idx_t)0;
    seq_t val    = (seq_t)0;
    while (1) {
        if (has_equal_345(val)) {
            ++found;
            if (found == n)
                return val;
        }
        ++val;
    }
}
