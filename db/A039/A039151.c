/*
 * A039151  Numbers whose base-10 representation has the same number of 6's
 *          and 8's.
 * https://oeis.org/A039151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th nonnegative integer whose decimal representation
 *            contains an equal number of 6's and 8's
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool equal_6_8(seq_t x)
{
    unsigned cnt6 = 0, cnt8 = 0;
    do {
        uint8_t d = (uint8_t)(x % (seq_t)10);
        if (d == 6) ++cnt6;
        else if (d == 8) ++cnt8;
        x /= (seq_t)10;
    } while (x);
    return cnt6 == cnt8;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1, but guard against zero */
    idx_t found = 0;
    seq_t cand = (seq_t)0;
    for (;; ++cand) {
        if (equal_6_8(cand)) {
            ++found;
            if (found == n) return cand;
        }
    }
}
