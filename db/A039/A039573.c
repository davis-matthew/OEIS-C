/*
 * A039573  Numbers whose base-6 representation has the same number of 0's,
 *          1's and 2's.
 * https://oeis.org/A039573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth integer whose base‑6 representation contains
 *            equal numbers of digits 0, 1, and 2 (A039573)
 * Range:     u8 n <= 49, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (idx_t x = 1; ; ++x) {
        unsigned c0 = 0, c1 = 0, c2 = 0;
        idx_t y = x;
        while (y > 0) {
            uint8_t d = y % 6;
            if (d == 0) {
                ++c0;
            } else if (d == 1) {
                ++c1;
            } else if (d == 2) {
                ++c2;
            }
            y /= 6;
        }
        if (c0 == c1 && c1 == c2) {
            ++found;
            if (found == n) {
                return (seq_t)x;
            }
        }
    }
}
