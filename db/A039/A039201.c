/*
 * A039201  Numbers whose base-11 representation has the same number of 6's
 *          and 8's.
 * https://oeis.org/A039201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   equal6and8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-11 representation has equal counts of digit 6 and digit 8. */
#include <stdbool.h>
#include "../utils/equal6and8.h"

seq_t a(idx_t n) {
    idx_t found = 0;
    seq_t x = 0;
    while (true) {
        if (equal6and8(x)) {
            ++found;
            if (found == n)
                return x;
        }
        ++x;
    }
}
