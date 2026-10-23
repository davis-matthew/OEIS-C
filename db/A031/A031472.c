/*
 * A031472  Numbers whose base-5 representation has 2 more 0's than 4's.
 * https://oeis.org/A031472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose base‑5 representation
 *            contains two more 0’s than 4’s
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t x = 0;
    while (1) {
        idx_t zeros = 0, fours = 0;
        idx_t t = x;
        while (t > 0) {
            uint8_t d = t % 5;
            if (d == 0) ++zeros;
            else if (d == 4) ++fours;
            t /= 5;
        }
        if (zeros == fours + 2) {
            ++found;
            if (found == n) {
                return (seq_t)x;
            }
        }
        ++x;
    }
}
