/*
 * A168301  a(n) = (6*n + 3*(-1)^n - 1)/2.
 * https://oeis.org/A168301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 3*(-1)^n - 1)/2 Equivalent: if n is even, a(n) =
 *            3*n + 1 if n is odd, a(n) = 3*n - 2
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 */
    if ((n & 1) == 0)  /* even */
        return (seq_t)3 * n + (seq_t)1;
    else               /* odd */
        return (seq_t)3 * n - (seq_t)2;
}
