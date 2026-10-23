/*
 * A182188  A sequence of row differences for table A182119.
 * https://oeis.org/A182188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2) - 4 ; a(0)=1, a(1)=-1
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 12, i64 n <= 25, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)-1;

    /* Iterative computation using the recurrence. */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)-1;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (seq_t)6 * prev1 - prev2 - (seq_t)4;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
