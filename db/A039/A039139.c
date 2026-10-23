/*
 * A039139  Numbers whose base-10 representation has the same number of 3's
 *          and 8's.
 * https://oeis.org/A039139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th integer whose decimal representation contains the
 *            same number of 3's and 8's
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t i = 0;
    while (1) {
        int c3 = 0, c8 = 0;
        seq_t tmp = i;
        if (tmp == 0) {
            /* cnt3 = cnt8 = 0 for 0 */
        } else {
            while (tmp != 0) {
                int d = (int)(tmp % 10);
                if (d == 3) ++c3;
                else if (d == 8) ++c8;
                tmp /= 10;
            }
        }
        if (c3 == c8) {
            ++count;
            if (count == (seq_t)n) {
                return i;
            }
        }
        ++i;
    }
}
