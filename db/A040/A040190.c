/*
 * A040190  Continued fraction for sqrt(205).
 * https://oeis.org/A040190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=14; for n>0, a(n)=period[(n-1) mod 8] with period
 *            {3,6,1,4,1,6,3,28}
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)14;
    static const seq_t period[8] = {3, 6, 1, 4, 1, 6, 3, 28};
    return period[(size_t)((n - 1) & 7u)];
}
