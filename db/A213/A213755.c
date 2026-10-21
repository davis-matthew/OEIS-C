/*
 * A213755  Antidiagonal sums of the convolution array A213753.
 * https://oeis.org/A213755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (84 - 21*2^(n+2) + 23*n + 9*n*2^(n+2) - 3*n^2 - 2*n^3)/6
 *            Simplified to: a(n) = 2^(n+1)*(3*n-7) + (84 + 23*n - 3*n^2 -
 *            2*n^3)/6
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 55, u128 n <= 118,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* safety: offset is 1 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)9;

    /* --------------------------------------------------------------------- */
    /*  2^(n+1) * (3*n - 7)  contribution                                 */
    seq_t pow_part = pow2(n + 1);           /* 2^(n+1) */
    seq_t coeff   = ((seq_t)3 * n) - 7;     /* 3*n - 7  */
    seq_t big_part = pow_part * coeff;      /* may be large, fits seq_t */

    /*  (84 + 23*n - 3*n^2 - 2*n^3)/6  contribution                      */
    __int128 small =
        (__int128)84 + (__int128)23 * n
        - (__int128)3 * n * n
        - (__int128)2 * n * n * n;
    small /= 6;                             /* integer division truncates toward zero */

    /*  Combine the two parts, taking care of possible negativity of the small part   */
    seq_t result;
    if (small >= 0)
        result = big_part + (seq_t)small;
    else
        result = big_part - (seq_t)(-small);

    return result;
}
