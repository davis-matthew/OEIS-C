/*
 * A037559  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 2,1,1.
 * https://oeis.org/A037559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(211 * 10^n / 999) (equivalently: digits 2,1,1
 *            repeated)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t cur = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t digit = ((i - 1) % 3 == 0) ? (seq_t)2 : (seq_t)1;
        cur = cur * (seq_t)10 + digit;
    }
    return cur;
}
