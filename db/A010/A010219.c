/*
 * A010219  Continued fraction for sqrt(175).
 * https://oeis.org/A010219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A010219: continued fraction expansion of sqrt(175).
   a(0) = 13, and for n > 0 the terms repeat the period
   [4, 2, 1, 2, 4, 26]. */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[6] = { 4, 2, 1, 2, 4, 26 };

    if (n == 0)
        return (seq_t)13;

    /* n >= 1 here; (n - 1) is safe for unsigned n */
    size_t idx = (size_t)((n - 1) % 6);
    return period[idx];
}
