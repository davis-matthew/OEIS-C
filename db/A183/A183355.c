/*
 * A183355  One quarter the number of nX3 1..4 arrays with no two neighbors of
 *          any element equal to each other.
 * https://oeis.org/A183355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=12, a(2)=144; a(n)=144*3^(n-2) for n>=2
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 12;          /* a(1) = 12  */
    if (n == 2)
        return 144;         /* a(2) = 144 */
    /* n>=3: a(n) = 144 * 3^(n-2) */
    return seq_pow((seq_t)3, n - 2) * 144;
}
