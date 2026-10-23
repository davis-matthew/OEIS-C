/*
 * A160811  Numbers not dividing 24.
 * https://oeis.org/A160811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A160811: numbers that do not divide 24.
   Formula: a(n) = n + 8 for n > 16
   The first 16 terms are handled by a static lookup table. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* First 16 terms of the sequence. */
    static const seq_t first16[16] = {
        5, 7, 9, 10, 11, 13, 14, 15,
        16, 17, 18, 19, 20, 21, 22, 23
    };

    if (n <= 16)
        return first16[(size_t)(n - 1)];
    else
        return (seq_t)n + (seq_t)8;
}
