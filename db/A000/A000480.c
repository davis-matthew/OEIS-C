/*
 * A000480  a(n) = floor(cos(n)).
 * https://oeis.org/A000480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = floor(cos(n)); i.e. a(0) = 1, otherwise 0 if cos(n) > 0
 *            and -1 if cos(n) < 0
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    int r;
    if (n == 0) {
        r = 1;
    } else {
        double c = cos((double)n);
        r = (c > 0.0) ? 0 : -1;
    }
    return (seq_t)r;
}
