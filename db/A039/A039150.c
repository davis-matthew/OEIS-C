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
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool equal_six_seven(seq_t x)
{
    unsigned c6 = 0, c7 = 0;
    seq_t y = x;
    while (y != (seq_t)0) {
        int d = (int)(y % (seq_t)10);
        if (d == 6)
            ++c6;
        else if (d == 7)
            ++c7;
        y /= (seq_t)10;
    }
    return c6 == c7;
}

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
