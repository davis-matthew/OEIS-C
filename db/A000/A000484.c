/*
 * A000484  a(n) = round(cos(n)).
 * https://oeis.org/A000484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = round(cos(n)); cos is evaluated on the index n in double
 *            precision, and the result (always -1, 0 or 1) is converted to
 *            seq_t through an int
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    double c = cos((double)n);
    int r;
    if (c > 0.5) r = 1;
    else if (c < -0.5) r = -1;
    else r = 0;
    return (seq_t)r;
}
