/*
 * A159807  Numerator of Hermite(n, 3/22).
 * https://oeis.org/A159807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=3; for n>=2, a(n) = 3*a(n-1) - 242*(n-1)*a(n-2)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t prev2 = (seq_t)1;           /* a(n-2) */
    seq_t prev1 = (seq_t)3;           /* a(n-1) */
    seq_t curr  = prev1;              /* will hold a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)3 * prev1
               - (seq_t)242 * (seq_t)(i - 1) * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
