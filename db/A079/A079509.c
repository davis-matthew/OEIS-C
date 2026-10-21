/*
 * A079509  Duplicate of A000096.
 * https://oeis.org/A079509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n*(n+3)/2
 * Range:     i8 n <= 14, i16 n <= 18, i32 n <= 18, i64 n <= 18, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    return (seq_t)n * ((seq_t)n + 3) / 2;
}
