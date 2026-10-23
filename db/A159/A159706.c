/*
 * A159706  Numerator of Hermite(n, 2/21).
 * https://oeis.org/A159706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 10, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A159706: Numerator of Hermite(n, 2/21)
   Recurrence: a(0)=1, a(1)=4
                a(n) = 4*a(n-1) - 882*(n-1)*a(n-2)   for n≥2 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)4;   /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        curr = (seq_t)4 * prev1
              - (seq_t)882 * (seq_t)(i - 1) * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
