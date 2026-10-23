/*
 * A159533  Numerator of Hermite(n, 5/17).
 * https://oeis.org/A159533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 17^n * Hermite(n, 5/17). Equivalent recurrence: a(0)=1,
 *            a(1)=10, a(n+1)=10*a(n)-578*n*a(n-1)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)10;  /* a(1) */
    seq_t cur;

    for (idx_t i = 1; i < n; ++i)          /* i = n in the recurrence a(n+1) = ... */
    {
        cur = (seq_t)10 * prev1
            - (seq_t)578 * (seq_t)i * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;   /* a(n) */
}
