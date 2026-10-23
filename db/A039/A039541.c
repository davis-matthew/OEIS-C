/*
 * A039541  Numbers whose base-12 representation has the same nonzero number
 *          of 5's and 9's.
 * https://oeis.org/A039541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth integer whose base‑12 representation contains the
 *            same nonzero number of 5’s and 9’s. The function scans integers
 *            sequentially and counts those that satisfy this property
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_equal_nonzero_5_and_9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_equal_nonzero_5_and_9.h"

seq_t a(idx_t n)
{
    seq_t found = 0;
    seq_t count = 0;
    seq_t candidate = 1;

    while (true) {
        if (has_equal_nonzero_5_and_9(candidate)) {
            ++count;
            if (count == n) {
                found = candidate;
                break;
            }
        }
        ++candidate;
    }
    return found;
}
