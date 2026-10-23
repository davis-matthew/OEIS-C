/*
 * A177230  a(n) = determinant of n X n circulant matrix whose first row
 *          consists of the first n squares (beginning with 1).
 * https://oeis.org/A177230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^(n-1)*(n+1)*(2*n+1)*n^(n-2)*((n+2)^n - n^n)/12
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* sign = (-1)^(n-1) */
    seq_t sign = (n & 1) ? (seq_t)1 : (seq_t)-1;

    /* (n+1)*(2*n+1) */
    seq_t term3 = ((seq_t)n + 1) * ((((seq_t)n) * 2) + 1);

    /* n^(n-2) */
    seq_t npow = big_pow((seq_t)n, (seq_t)(n - 2));

    /* (n+2)^n - n^n */
    seq_t pow_n_plus2 = big_pow((seq_t)n + 2, (seq_t)n);
    seq_t pow_n      = big_pow((seq_t)n, (seq_t)n);
    seq_t diff       = pow_n_plus2 - pow_n;

    /* Assemble */
    seq_t result = sign;
    result *= term3;
    result *= npow;
    result *= diff;
    result /= (seq_t)12;

    return result;
}
