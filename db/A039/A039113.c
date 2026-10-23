/*
 * A039113  Numbers whose base-10 representation has the same number of 0's
 *          and 2's.
 * https://oeis.org/A039113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   compute the nth integer whose decimal representation contains
 *            the same number of 0's and 2's. Brute‑force scan
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   has_equal_zero_and_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_equal_zero_and_two.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t val   = 1;
    while (count < n) {
        if (has_equal_zero_and_two(val)) {
            ++count;
            if (count == n) {
                return val;
            }
        }
        ++val;
    }
    return 0; /* Should never happen for valid n */
}
