/*
 * A131713  Period 3: repeat [1, -2, 1].
 * https://oeis.org/A131713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = a(n-3) for n>2, a(0)=1, a(1)=-2, a(2)=1 (period 3:
 *            [1,-2,1] repeating)
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Periodic block of the sequence. */
    static const seq_t period[3] = { (seq_t)1, (seq_t)-2, (seq_t)1 };

    /* Use modulo 3 to index into the period. */
    return period[(size_t)(n % 3)];
}
