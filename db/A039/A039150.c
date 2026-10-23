/*
 * A039150  Numbers whose base-10 representation has the same number of 6's
 *          and 7's.
 * https://oeis.org/A039150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th nonnegative integer whose decimal digits *
 *            contain an equal number of 6's and 7's (found by a linear
 *            search)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   equal_six_seven
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/equal_six_seven.h"

seq_t a(idx_t n)
{
    idx_t remaining = n;          /* number of terms left to skip */
    seq_t cur = (seq_t)0;         /* current candidate integer */

    while (1) {
        if (equal_six_seven(cur)) {
            if (remaining == (idx_t)1)
                return cur;
            --remaining;
        }
        ++cur;
    }
}
