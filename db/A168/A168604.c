/*
 * A168604  a(n) = 2^(n-2) - 1.
 * https://oeis.org/A168604
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-2) - 1
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;          /* behaviour below the offset is not required */
    idx_t exp = n - 2;
    seq_t result = pow2(exp); /* 2^(n-2) */
    result -= 1;             /* subtract 1 */
    return result;
}
