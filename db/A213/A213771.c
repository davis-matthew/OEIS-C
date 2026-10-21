/*
 * A213771  Rectangular array: (row n) = b**c, where b(h) = 3*h-2, c(h) =
 *          n-1+h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(r,k) = ( (r+k) * k * (3*k-1) / 2 ) - ( k * (k+1) * (2*k-1) / 2
 *            ), where r = row index, k = column index (both >=1). The
 *            sequence a(n) is read by falling antidiagonals: a(1) = T(1,1),
 *            a(2) = T(1,2), a(3) = T(2,1), a(4) = T(1,3),
 * Range:     u8 n <= 28, u16 n <= 1275, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* find diagonal d such that T_{d-1} < n <= T_d with T_d = d*(d+1)/2 */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n)
        ++d;

    /* first index on diagonal d */
    idx_t start = (d - 1) * d / 2 + 1;
    /* position on the diagonal (1-based) */
    idx_t pos = n - start + 1;

    /* coordinates: row = pos, column = d - pos + 1 */
    seq_t R = (seq_t)pos;
    seq_t K = (seq_t)(d - pos + 1);

    /* compute T(r,k) using integer arithmetic */
    seq_t part1 = (R + K) * K * (3 * K - 1) / 2;
    seq_t part2 = K * (K + 1) * (2 * K - 1) / 2;

    return part1 - part2;
}
