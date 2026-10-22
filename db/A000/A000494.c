/*
 * A000494  Nearest integer to sin(n).
 * https://oeis.org/A000494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = round(sin(n)); sin is evaluated on the index n, and the
 *            result is always -1, 0 or 1
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    double s = sin((double)n);
    int r;
    if (s >= 0.5)
        r = 1;
    else if (s <= -0.5)
        r = -1;
    else
        r = 0;
    return (seq_t)r;
}
