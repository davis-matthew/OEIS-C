/*
 * A043911  Numbers k such that 0 and 1 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   has_adjacent_zero_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: enumerate integers k and collect those whose base-7 representation
   contains the digits 0 and 1 adjacent (in either order) but whose base-7
   representation of k+1 does not.  The n-th such k is returned. */
#include <stdbool.h>
#include "../utils/has_adjacent_zero_one.h"

seq_t a(idx_t n)
{
    idx_t count = 0;                      /* how many qualifying numbers found */
    seq_t k = 0;                          /* current candidate */

    while (count < n) {
        if (has_adjacent_zero_one(k) &&
            !has_adjacent_zero_one(k + (seq_t)1)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
    /* The loop above always returns before reaching this point */
    return (seq_t)0;
}
