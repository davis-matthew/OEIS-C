/*
 * A162285  Period 8, repeat: [1, -1, -1, 1, -1, 1, 1, -1].
 * https://oeis.org/A162285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = pattern of period 8: [1, -1, -1, 1, -1, 1, 1, -1]
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pat[8] = {1, -1, -1, 1, -1, 1, 1, -1};
    return pat[(size_t)(n & 7ULL)];
}
