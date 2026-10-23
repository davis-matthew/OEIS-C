/*
 * A039198  Numbers whose base-11 representation has the same number of 5's
 *          and 9's.
 * https://oeis.org/A039198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th non‑negative integer whose base‑11
 *            representation contains an equal number of digits 5 and 9. The
 *            first term is a(1)=0
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   has_equal_59
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_equal_59.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t  num   = (seq_t)0;

    for (;;) {
        if (has_equal_59(num)) {
            ++found;
            if (found == n)
                return num;
        }
        ++num;
    }
}
