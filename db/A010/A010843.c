/*
 * A010843  Incomplete Gamma Function at -3.
 * https://oeis.org/A010843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=1; for n>1: a(n)= (n-1)*a(n-1)+(-3)^(n-1) (OEIS A010843)
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 14, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n<1 */

    idx_t k;
    seq_t acc = (seq_t)1;                /* a(1) corresponds to a(0) in recurrence */
    seq_t pow = (seq_t)1;                /* (-3)^0 */

    for (k = 1; k < n; ++k) {
        pow *= (seq_t)(-3);              /* advance exponent: (-3)^k */
        acc = (seq_t)k * acc + pow;     /* compute next value */
    }

    return acc;
}
