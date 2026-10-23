/*
 * A039523  Numbers whose base-12 representation has the same nonzero number
 *          of 3's and 4's.
 * https://oeis.org/A039523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth positive integer whose base‑12 representation
 *            contains * an equal, nonzero number of digits 3 and 4
 * Range:     u8 n <= 4, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   has_equal34
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_equal34.h"

seq_t a(idx_t n) {
    idx_t found = 0;
    idx_t val   = 0;
    while (1) {
        if (has_equal34(val)) {
            ++found;
            if (found == n)
                return (seq_t)val;
        }
        ++val;
    }
}
