/*
 * A122685  a(n) = n! except that a(2) = -2 and a(2n) = 0 for n > 2.
 * https://oeis.org/A122685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = n! except a(2) = -2 and a(2n)=0 for n>2
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 29,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0)                 return (seq_t)1;
    if (n == 1)                 return (seq_t)1;
    if (n == 2)                 return (seq_t)-2;
    if ((n & 1) == 0)           return (seq_t)0;   /* even > 2 */
    return factorial(n);        /* odd n (≥3) */
}
