/*
 * A140289  First quadrisection of A140287.
 * https://oeis.org/A140289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -7*a(n-1) -17*a(n-2) -15*a(n-3) a(0) = 0, a(1) = 0, a(2)
 *            = 1
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 20, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;

    /* Compute iteratively using only the last three values */
    seq_t a0 = 0;   /* a(n-3) */
    seq_t a1 = 0;   /* a(n-2) */
    seq_t a2 = 1;   /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)-7 * a2 + (seq_t)-17 * a1 + (seq_t)-15 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2;
}
