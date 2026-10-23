/*
 * A144143  a(n) = Hermite(n,4).
 * https://oeis.org/A144143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Hermite(n,4) Recurrence: a(0)=1, a(1)=8, for n≥2: a(n) =
 *            8*a(n-1) - 2*(n-1)*a(n-2) (physicists' Hermite polynomial)
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 12, i64 n <= 24, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle small n directly (the recurrence uses a(0) and a(1)). */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)8;

    /* Iterative computation using the recurrence. */
    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)8;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2u; i <= n; ++i)
    {
        /* Compute 8*prev1 - 2*(i-1)*prev2 */
        cur = (seq_t)8 * prev1 - (seq_t)2 * (seq_t)(i - 1u) * prev2;
        /* Slide window */
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
