/*
 * A057054  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...;
 *          n^3 is in antidiagonal a(n).
 * https://oeis.org/A057054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n*sqrt(2*n))
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   _isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_isqrt_seq.h"

seq_t a(idx_t n)
{
    /* Convert n to seq_t */
    seq_t seq_n = (seq_t)n;

    /* v = 2 * n^3 */
    seq_t v = seq_n * seq_n * seq_n * (seq_t)2;

    /* r = floor(sqrt(v)) */
    seq_t r = _isqrt_seq(v);

    /* r_sq = r * r */
    seq_t r_sq = r * r;

    /* d = v - r_sq  (0 <= d < diff) */
    seq_t d = v - r_sq;

    /* diff = (r+1)^2 - r^2 = 2*r + 1 */
    seq_t diff = (r << 1) + (seq_t)1;

    /* two_d = 2*d */
    seq_t two_d = d << 1;

    /* If fractional part >= 0.5 -> round up */
    if (two_d >= diff)
        return r + (seq_t)1;
    else
        return r;
}
