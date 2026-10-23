/*
 * A132798  Period 6: repeat [0, 2, 1, 0, -2, -1].
 * https://oeis.org/A132798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-n mod 3) * (-1)^floor(n/3) (period 6: 0,2,1,0,-2,-1)
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* lookup table for the length‑6 period */
    static const seq_t tbl[6] = {0, 2, 1, 0, -2, -1};
    return tbl[(size_t)(n % 6)];
}
