/*
 * A097506  Duplicate of A001951.
 * https://oeis.org/A097506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = ⌊n·√2⌋
 * Range:     i8 n <= 5, i16 n <= 25, i32 n <= 25, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented as a(n) = ⌊√(2⋅n²)⌋ using the provided floor_sqrt function. */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute 2·n² and take its integer square root. */
    seq_t t = nn * nn * (seq_t)2;
    return floor_sqrt(t);
}
