/*
 * A135447  Period 10: repeat [1, 2, 4, 8, 5, -1, -2, -4, -8, -5].
 * https://oeis.org/A135447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(0)=1; a(n+1)=2*a(n) mod 11 gives a periodic sequence of period
 *            10: [1, 2, 4, 8, 5, -1, -2, -4, -8, -5]
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The sequence repeats every 10 terms, so we can index a
       static lookup table modulo 10. */
    static const seq_t pattern[10] = {
        1,  2,  4,  8,  5, -1, -2, -4, -8, -5
    };
    return pattern[(size_t)(n % 10)];
}
