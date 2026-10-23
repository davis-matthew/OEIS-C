/*
 * A133572  Row sums of triangle A133571.
 * https://oeis.org/A133572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n if n is even, a(n) = 2^(n-1) + 2^(n-2) if n is odd.
 *            (For n=1: a(1)=1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* guard against n < 1 */

    if ((n & 1ULL) == 0) {                 /* even index */
        return pow2(n);
    } else {                               /* odd index */
        if (n == 1) return pow2(0);       /* a(1) = 1 */
        return pow2(n - 1) + pow2(n - 2);
    }
}
