/*
 * A039217  Numbers whose base-12 representation has the same number of 0's
 *          and 8's.
 * https://oeis.org/A039217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth positive integer whose base‑12 representation
 *            contains an equal number of the digits 0 and 8
 * Range:     u8 n <= 186, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   balanced_zeros_eights
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/balanced_zeros_eights.h"

seq_t a(idx_t n) {
    /* Count of valid numbers found so far */
    idx_t count = 0;
    seq_t val = (seq_t)1;

    while (true) {
        if (balanced_zeros_eights(val)) {
            ++count;
            if (count == n) {
                return val;
            }
        }
        ++val;
    }
}
