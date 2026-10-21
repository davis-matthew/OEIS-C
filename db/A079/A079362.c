/*
 * A079362  Sequence of sums of alternating powers of 3.
 * https://oeis.org/A079362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2n-1) = (2/3)*3^n - 1, a(2n) = (5/3)*3^n - 1
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 79, u128 n <= 161,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* below the offset, undefined */
    idx_t e = (n - 1) / 2;        /* exponent of 3 */
    seq_t p = pow3(e);            /* compute 3^e */
    seq_t coef = (n % 2 == 1) ? 2 : 5; /* 2 for odd, 5 for even */
    seq_t res = p * coef - 1;     /* c * 3^e - 1 */
    return res;
}
