/*
 * A168615  Inverse binomial transform of A169609, or of A144437 preceded by
 *          1.
 * https://oeis.org/A168615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -3*a(n-1) - 3*a(n-2) for n>2; a(0) = 1, a(1) = 2, a(2) =
 *            -2
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 39, i64 n <= 81, i128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the few initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)-2;

    /* Use the recurrence for the rest of the sequence. */
    seq_t prev2 = (seq_t)2;      /* a(1) */
    seq_t prev1 = (seq_t)-2;     /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)-3 * prev1 - (seq_t)3 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
