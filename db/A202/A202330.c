/*
 * A202330  Number of (n+1) X 4 binary arrays with consecutive windows of two
 *          bits considered as a binary number nondecreasing in every row and
 *          column.
 * https://oeis.org/A202330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 16*n^3 + 83*n^2 + 176*n + 156) / 12 (equivalent to
 *            the recurrence a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) -
 *            5*a(n-4) + a(n-5) with initial values a(1)=36, a(2)=82,
 *            a(3)=162, a(4)=289, a(5)=478.)
 * Range:     u8 none, u16 n <= 12, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Offset is 1; for n==0 the value is undefined, return 0 */
    if (n == 0) return (seq_t)0;

    /* All arithmetic is performed on seq_t, which is an unsigned integer type. */
    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;          /* n^2          */
    seq_t n3  = n2 * nn;          /* n^3          */
    seq_t n4  = n3 * nn;          /* n^4          */

    /* Numerator of the rational expression: n^4 + 16*n^3 + 83*n^2 + 176*n + 156 */
    seq_t num = n4 +  (seq_t)16 * n3 + (seq_t)83 * n2 + (seq_t)176 * nn + (seq_t)156;

    /* Denominator is 12; division is exact for all integer n ≥ 1. */
    return num / (seq_t)12;
}
