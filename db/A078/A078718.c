/*
 * A078718  a(n) = (-1)^n*(2*n - 1)*CatalanNumber(n - 2) for n >= 2, a(n) = n
 *          for n = 0, 1.
 * https://oeis.org/A078718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n for n = 0,1; a(n) = (-1)^n * (2*n - 1) * Catalan(n-2)
 *            for n >= 2
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 18, i64 n <= 34, i128 n <= 66,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* Compute Catalan number C_{n-2} */
    seq_t cat = catalan(n - 2);

    /* Compute (2*n - 1) */
    seq_t nseq   = (seq_t)n;
    seq_t two_n_minus_1 = 2 * nseq - (seq_t)1;

    /* Multiply and apply sign: (-1)^n */
    seq_t result = two_n_minus_1 * cat;
    if (n & 1)
        result = -result;

    return result;
}
