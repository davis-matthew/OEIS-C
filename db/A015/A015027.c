/*
 * A015027  q-factorial numbers for q=-12.
 * https://oeis.org/A015027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1; a(n) = ((-12)^n - 1) * a(n-1) / (-13)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 4, i64 n <= 6, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the base case n = 0 directly. */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* Current value of the sequence and current power of -12. */
    seq_t current_a = (seq_t)1;           /* a(0) */
    seq_t current_pow = (seq_t)1;         /* (-12)^0 */

    /* Iterate from 1 up to n computing the recurrence. */
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        /* Compute (-12)^i from the previous power. */
        current_pow = current_pow * (seq_t)(-12);

        /* numerator = (-12)^i - 1, denominator = -13. */
        seq_t numerator   = current_pow - (seq_t)1;
        seq_t denominator = (seq_t)(-13);

        /* Update a(n) using the recurrence. */
        current_a = (numerator * current_a) / denominator;
    }

    return current_a;
}
