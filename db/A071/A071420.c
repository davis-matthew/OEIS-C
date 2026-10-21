/*
 * A071420  Duplicate of A031139.
 * https://oeis.org/A071420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) repeats with period 12. For n = 1..12 the values are 7, 8,
 *            5, 5, 3, 4, 4, 6, 9, 7, 8, 8. Thus a(n) = table[(n‑1) % 12]
 * Range:     i8 n <= 33, i16 n <= 33, i32 n <= 33, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    static const seq_t table[12] = {7, 8, 5, 5, 3, 4, 4, 6, 9, 7, 8, 8};
    idx_t r = (n - 1) % 12;   /* n >= 1 by specification */
    return table[(size_t)r];
}
