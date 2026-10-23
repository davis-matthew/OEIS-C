/*
 * A174647  Determinant of the matrix a(i,j) = 1 if abs(i-j) < 2 and -1 in the
 *          rest.
 * https://oeis.org/A174647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 0 if n % 3 == 2; a(n) = 2^(n-1) * det(T_n) * (2 -
 *            floor((n+2)/3)) where det(T_n) = 1 if n % 6 == 0 or 1, -1
 *            otherwise
 * Range:     i8 n <= 8, i16 n <= 14, i32 n <= 29, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* a(0) = 1 */
        return (seq_t)1;

    if ((n % 3) == 2)          /* a(n) = 0 for n ≡ 2 (mod 3) */
        return (seq_t)0;

    idx_t s = (n + 2) / 3;      /* floor((n+2)/3)            */
    seq_t pow2 = pow_seq((seq_t)2, n - 1);   /* 2^(n-1) */

    seq_t detT;
    idx_t r = n % 6;
    if (r == 0 || r == 1)
        detT = (seq_t)1;
    else
        detT = (seq_t)-1;

    seq_t mult = (seq_t)2 - (seq_t)s;  /* 2 - floor((n+2)/3) */

    return pow2 * detT * mult;
}
