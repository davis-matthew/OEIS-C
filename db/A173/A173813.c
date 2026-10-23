/*
 * A173813  2*(10^n-5), n>0.
 * https://oeis.org/A173813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(10^n-5)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* The sequence is only defined for n >= 1. */
    if (n == 0u)
        return 0;          /* safe guard, value irrelevant for the offset 1 */
    seq_t v = pow10(n);    /* compute 10^n as seq_t */
    v -= 5;                /* subtract 5 */
    v <<= 1;               /* multiply by 2 */
    return v;
}
