/*
 * A219977  Expansion of 1/(1+x+x^2+x^3).
 * https://oeis.org/A219977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = [1, -1, 0, 0][n mod 4]
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t period[4] = {1, -1, 0, 0};
    return period[(size_t)(n % 4)];
}
