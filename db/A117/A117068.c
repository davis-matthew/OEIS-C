/*
 * A117068  Decimal value of binary number whose n-th 1 is followed by n^2
 *          0's.
 * https://oeis.org/A117068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = integer obtained by writing the pattern "1" followed by
 *            n² zeros, repeated for i = 1 … n and ending with a final “1”;
 *            the decimal value of that binary string is a(n)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t val = 1;                /* initial leading '1'   */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t shift = i * i + 1;  /* i² zeros + one 1     */
        val <<= shift;
        val |= 1;
    }
    return val;
}
