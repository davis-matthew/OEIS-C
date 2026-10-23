/*
 * A115357  Period 6: repeat [1, 1, 1, 0, 2, 0].
 * https://oeis.org/A115357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = [1, 1, 1, 0, 2, 0] repeated with period 6 (offset 0)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[6] = {1, 1, 1, 0, 2, 0};
    size_t r = (size_t)(n % 6);
    return period[r];
}
