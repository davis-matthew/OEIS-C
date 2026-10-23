/*
 * A164683  a(n) = 8*a(n-2) for n > 2; a(1) = 1, a(2) = 8.
 * https://oeis.org/A164683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-2), a(1) = 1, a(2) = 8
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;

    idx_t exp;
    if ((n & 1) == 0)            /* even n: a(2k) = 8^k                     */
        exp = n / 2;
    else                          /* odd  n: a(2k-1) = 8^(k-1)               */
        exp = (n - 1) / 2;

    return pow_uint(8, exp);
}
