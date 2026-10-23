/*
 * A049072  Expansion of 1/(1 - 3*x + 4*x^2).
 * https://oeis.org/A049072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2); a(0)=1, a(1)=3 * This implements the
 *            linear recurrence for the expansion of * 1/(1 - 3*x + 4*x^2).
 *            The calculation is O(n) per call and * uses only the integer
 *            arithmetic required by seq_t
 * Range:     i8 n <= 8, i16 n <= 14, i32 n <= 30, i64 n <= 62, i128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first two values explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)3;  /* a(1) */
    seq_t current;

    /* Build up to a(n) using the recurrence. */
    for (idx_t i = 2; i <= n; ++i) {
        current = (seq_t)(3 * prev1 - 4 * prev2);
        prev2 = prev1;
        prev1 = current;
    }

    return prev1;
}
