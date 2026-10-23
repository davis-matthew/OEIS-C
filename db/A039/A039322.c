/*
 * A039322  Numbers whose base-7 representation has the same nonzero number of
 *          2's and 6's.
 * https://oeis.org/A039322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   has_equal_nonzero_2_6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers whose base-7 representation has equal nonzero counts of
   2 and 6. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_equal_nonzero_2_6.h"

seq_t a(idx_t n)
{
    seq_t candidate = 1;
    idx_t found = 0;
    while (found < n) {
        if (has_equal_nonzero_2_6(candidate))
            ++found;
        ++candidate;
    }
    return candidate - 1;
}
