/*
 * A118707  a(n) = determinant of n X n circulant matrix whose first row is
 *          the first n square numbers 0, 1, ..., (n-1)^2.
 * https://oeis.org/A118707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^(n-1)*(n-1)*(2*n-1)*n^(n-2)*(n^n-(n-2)^n)/12 a(0) =
 *            1, a(1) = 0
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the special first two values */
    if (n == 0) return 1;
    if (n == 1) return 0;

    seq_t sn          = (seq_t)n;           /* n as seq_t */
    seq_t n_minus_1   = sn - 1;             /* n-1 */
    seq_t two_n_minus_1 = 2 * sn - 1;       /* 2n-1 */

    /* n^(n-2) */
    seq_t n_pow_n_minus_2 = big_pow(sn, sn - 2);

    /* factor = (n-1)*(2n-1)*n^(n-2) */
    seq_t factor = n_minus_1 * two_n_minus_1 * n_pow_n_minus_2;

    /* n^n and (n-2)^n */
    seq_t n_pow_n       = big_pow(sn, sn);
    seq_t n_minus_2     = sn - 2;           /* safe for n>=2 */
    seq_t n_minus_2_pow_n = big_pow(n_minus_2, sn);

    /* diff = n^n - (n-2)^n */
    seq_t diff = n_pow_n - n_minus_2_pow_n;

    /* numerator = factor * diff */
    seq_t num = factor * diff;

    /* final result = numerator / 12 */
    seq_t result = num / 12;

    /* Apply the sign (-1)^(n-1) */
    if ((n & 1) == 0)                /* n even => (-1)^(n-1) = -1 */
        result = -result;

    return result;
}
