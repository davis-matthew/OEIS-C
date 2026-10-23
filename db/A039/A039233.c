/*
 * A039233  Numbers whose base-12 representation has the same number of 2's
 *          and 6's.
 * https://oeis.org/A039233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   has_equal_2_and_6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base-12 representation has the same number of 2's and 6's. */
/* a(1)=0, a(2)=1, a(3)=3, …, as defined in OEIS A039233. */

#include <stdbool.h>
#include "../utils/has_equal_2_and_6.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* out of range, but safe */
    idx_t target = n;
    idx_t found  = 0;
    seq_t cur = 0;
    while (found < target) {
        if (has_equal_2_and_6(cur)) ++found;
        if (found == target) return cur;
        ++cur;
    }
    return 0;                            /* unreachable */
}
