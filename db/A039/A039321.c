/*
 * A039321  Numbers whose base-7 representation has the same nonzero number of
 *          2's and 5's.
 * https://oeis.org/A039321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   equal_2_and_5_nonzero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base-7 representation has equal nonzero counts of 2 and 5 */
#include <stdlib.h>
#include "../utils/equal_2_and_5_nonzero.h"

seq_t a(idx_t n) {
    idx_t found = 0;
    seq_t candidate = 1;
    while (1) {
        if (equal_2_and_5_nonzero(candidate)) {
            ++found;
            if (found == n) return candidate;
        }
        ++candidate;
    }
}
