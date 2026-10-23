/*
 * A159946  Numerator of Hermite(n, 20/23).
 * https://oeis.org/A159946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 23^n * Hermite(n, 20/23) Recurrence: a(0) = 1, a(1) = 40;
 *            a(n) = 40*a(n-1) - 1058*(n-1)*a(n-2) for n ≥ 2
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)40;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)40;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* a(i) = 40*a(i-1) - 1058*(i-1)*a(i-2) */
        seq_t mult = (seq_t)1058 * (seq_t)(i - 1);
        seq_t term = (seq_t)40 * prev1;
        seq_t sub  = mult * prev2;
        seq_t cur  = term - sub;

        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
