/*
 * A213574  Principal diagonal of the convolution array A213573.
 * https://oeis.org/A213574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(n^2+2n+3) - (3+4n+4n^2)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 52, u128 n <= 114,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) /* safety: OEIS offset starts at 1 */
        return (seq_t)0;

    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;

    seq_t poly1 = n2 + 2 * nn + (seq_t)3;          /* n^2 + 2n + 3 */
    seq_t poly2 = 4 * n2 + 4 * nn + (seq_t)3;      /* 4n^2 + 4n + 3 */

    seq_t term1 = pow2(n) * poly1;                 /* 2^n * poly1 */

    return term1 - poly2;
}
