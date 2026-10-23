/*
 * A174316  Sequence defined by a(0)=a(1)=a(2)=1, a(3)=2, a(4)=6 and the
 *          formula a(n)=2^(n-2)+2 for n>=5.
 * https://oeis.org/A174316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-2)+2 for n >= 5, a(0) = a(1) = a(2) = 1, a(3) = 2,
 *            a(4) = 6
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)2;
    if (n == 4)
        return (seq_t)6;
    /* n >= 5 */
    return pow2(n - 2) + (seq_t)2;
}
