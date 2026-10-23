/*
 * A037785  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 3,1,2,0.
 * https://oeis.org/A037785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 3; for n > 1, a(n) = 10 * a(n-1) + digit(n) with digit
 *            sequence 3,1,2,0 repeating
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const uint8_t pat[4] = {3, 1, 2, 0};
    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i) {
        res = res * 10 + pat[(i - 1) % 4];
    }
    return res;
}
