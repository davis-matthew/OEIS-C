/*
 * A130848  Periodic sequence with period (2, 5, 3, -2, -5, -3).
 * https://oeis.org/A130848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-6) with a(0)=2, a(1)=5, a(2)=3, a(3)=-2, a(4)=-5,
 *            a(5)=-3
 * Range:     i8 n <= 101, i16 n <= 101, i32 n <= 101, i64 n <= 101,
 *            i128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* periodic part of length 6 */
    static const seq_t period[6] = { 2, 5, 3, -2, -5, -3 };

    /* compute n mod 6 (use an unsigned constant to avoid signed/unsigned mix warnings) */
    idx_t r = n % 6UL;

    /* index into the period array */
    return period[(size_t)r];
}
