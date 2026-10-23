/*
 * A039441  Numbers whose base-11 representation has the same nonzero number
 *          of 0's and 4's.
 * https://oeis.org/A039441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   equal_zero_four
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A039441: numbers whose base-11 representation has the same
 * nonzero number of 0's and 4's.
 */
#include <stdbool.h>
#include "../utils/equal_zero_four.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cur = (seq_t)1;  /* start from 1, as 0 cannot satisfy the condition */

    while (1) {
        if (equal_zero_four(cur)) {
            ++found;
            if (found == n) return cur;
        }
        ++cur;  /* next integer */
    }
}
