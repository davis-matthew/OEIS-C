/*
 * A146161  a(n) is the number of n X n matrices with entries in {1,2,3} such
 *          that all adjacent entries (in the same row or column) differ by 1
 *          or -1.
 * https://oeis.org/A146161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^((n^2+2)/2) if n is even 3*2^((n^2-1)/2) if n is odd
 *            (offset 1, start: a(1)=3)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n<1 – return 0 */

    idx_t n2 = n * n;              /* n^2 fits in idx_t for all n ≤ 10000 */

    if ((n & 1) == 0) {           /* even n */
        idx_t exp = (n2 + 2) / 2;
        return pow2(exp);
    } else {                      /* odd n */
        idx_t exp = (n2 - 1) / 2;
        return pow2(exp) * 3;
    }
}
