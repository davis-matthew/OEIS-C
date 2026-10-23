/*
 * A130031  Row sums of triangle A129467.
 * https://oeis.org/A130031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 8, i64 n <= 13, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A130031: Row sums of triangle A129467
   Formula:  a(0) = 1
   For n ≥ 1:  a(n) = - (n^2 - n - 1) * a(n-1)
   Equivalently:  a(n) = (-1)^n * ∏_{k=1..n} (k^2 - k - 1)          */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;                 /* a(0) = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        /* Compute the factor -(i^2 - i - 1) with type seq_t to avoid
           overflowing the native idx_t while n is large.  */
        seq_t i_seq   = (seq_t)i;
        seq_t factor  = -((i_seq * (i_seq - (seq_t)1)) - (seq_t)1);
        result *= factor;
    }

    return result;
}
