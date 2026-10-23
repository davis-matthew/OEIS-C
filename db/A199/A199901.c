/*
 * A199901  Number of -n..n arrays x(0..5) of 6 elements with zero sum, and
 *          adjacent elements not both strictly positive and not both strictly
 *          negative.
 * https://oeis.org/A199901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 40, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OAIEIS A199901
   Formula: a(n) = (11*n^5)/10 + (55*n^4)/6 + (55*n^3)/2 + (173*n^2)/6 + (37*n)/5 + 1
   The implementation evaluates the rational expression using a common
   denominator of 60 to avoid intermediate fractional values. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for this sequence, return 0 */

    /* Convert idx_t to seq_t for arbitrary‑precision arithmetic */
    seq_t N = (seq_t)n;

    /* Compute powers of n */
    seq_t n2 = N * N;          /* n^2 */
    seq_t n3 = n2 * N;         /* n^3 */
    seq_t n4 = n2 * n2;        /* n^4 */
    seq_t n5 = n4 * N;         /* n^5 */

    /* All coefficients multiplied by 60 to use a common denominator 60 */
    seq_t term1 = (seq_t)66   * n5;   /* 11*n^5 * 6   = 66*n^5   */
    seq_t term2 = (seq_t)550  * n4;   /* 55*n^4 *10   = 550*n^4  */
    seq_t term3 = (seq_t)1650 * n3;   /* 55*n^3 *30   = 1650*n^3 */
    seq_t term4 = (seq_t)1730 * n2;   /* 173*n^2*10   = 1730*n^2 */
    seq_t term5 = (seq_t)444   * N;    /* 37*n*12      = 444*N   */
    seq_t term6 = (seq_t)60;          /* constant 60  */

    /* Sum the weighted terms */
    seq_t numerator = term1 + term2;
    numerator += term3;
    numerator += term4;
    numerator += term5;
    numerator += term6;

    /* Divide by the common denominator 60 to obtain the integer value */
    return numerator / (seq_t)60;
}
