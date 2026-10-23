/*
 * A044362  Numbers n such that string 3,0 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has30
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A044362 – numbers n such that the string "30" occurs
   in the decimal representation of n but not in n-1.
   Computation is done by linear search; no hard‑coded table. */

#include <stdbool.h>
#include "../utils/has30.h"

seq_t a(idx_t n)
{
    /* Starting from 1, search forward for n numbers that satisfy the
       condition.  The search is independent for each call, which
       satisfies requirement 6 (any call order). */
    seq_t cur = (seq_t)0;
    idx_t found = 0;

    while (found < n) {
        cur++; /* advance to next integer */

        if (has30(cur) && !has30(cur - (seq_t)1))
            ++found;
    }
    return cur;
}
