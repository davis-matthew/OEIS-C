/*
 * A039339  Numbers whose base-8 representation has the same nonzero number of
 *          1's and 5's.
 * https://oeis.org/A039339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   has_equal_ones_and_fives_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based generation of numbers whose base‑8 representation
   has equal nonzero numbers of 1's and 5's (OEIS A039339). */

#include <stdbool.h>
#include "../utils/has_equal_ones_and_fives_base8.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but safe guard */

    idx_t found = 0;
    seq_t num = 1;                 /* start from 1 */

    for (;; ++num) {
        if (has_equal_ones_and_fives_base8(num)) {
            ++found;
            if (found == n)
                return num;
        }
    }
}
