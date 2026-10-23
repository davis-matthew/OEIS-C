/*
 * A040897  Continued fraction for sqrt(928).
 * https://oeis.org/A040897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040897: continued fraction terms of sqrt(928).
   a(0) = 30 (⌊√928⌋); for n ≥ 1 the sequence repeats the period
   2,6,3,1,1,1,8,15,8,1,1,1,3,6,2,60 of length 16. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* The first term is the integer part of sqrt(928). */
    if (n == 0)
        return (seq_t)30;

    /* Periodic part of the continued fraction. */
    static const seq_t period[16] = {
        2, 6, 3, 1, 1, 1, 8, 15,
        8, 1, 1, 1, 3, 6, 2, 60
    };

    /* Compute index into the period: (n-1) mod 16. */
    idx_t idx = (n - 1) % 16;
    return period[(size_t)idx];
}
