/*
 * A039549  Numbers whose base-12 representation has the same nonzero number
 *          of 7's and 8's.
 * https://oeis.org/A039549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   this function searches sequentially for the numbers whose
 *            base‑12 * representation contains the same non‑zero number of
 *            digit 7’s and digit 8’s * (and at least one of each). The
 *            sequence is returned in ascending order, * with a(1) = 92
 * Range:     u8 n <= 4, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_equal_nonzero_78
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_equal_nonzero_78.h"

seq_t a(idx_t n)
{
    /* n is 1‑based, returns a(n) */
    idx_t found = 0;
    seq_t num   = (seq_t)1;
    while (1) {
        if (has_equal_nonzero_78(num)) {
            found++;
            if (found == n) return num;
        }
        num++;
    }
}
