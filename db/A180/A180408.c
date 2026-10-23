/*
 * A180408  Nonzero digits not used in n.
 * https://oeis.org/A180408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 123456789 after removing any digits that appear in n. If
 *            n contains all digits 1–9, then a(n) = 0
 * Range:     u8 none, u16 none, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned char used[10] = {0};
    idx_t tmp = n;
    if (tmp != 0) {
        while (tmp > 0) {
            unsigned d = tmp % 10;
            if (d >= 1 && d <= 9)
                used[d] = 1;
            tmp /= 10;
        }
    }
    seq_t result = 0;
    for (unsigned d = 1; d <= 9; ++d) {
        if (!used[d])
            result = result * 10 + d;
    }
    return result;
}
