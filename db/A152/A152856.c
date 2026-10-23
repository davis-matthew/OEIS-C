/*
 * A152856  Periodic sequence [4,0,4,3,4] of period 5.
 * https://oeis.org/A152856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+5)=a(n); a(0)=4, a(1)=0, a(2)=4, a(3)=3, a(4)=4
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t values[5] = {4, 0, 4, 3, 4};
    return values[(size_t)(n % 5)];
}
