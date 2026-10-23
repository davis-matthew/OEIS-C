/*
 * A130815  Period 6: repeat [1, 5, 4, -1, -5, -4].
 * https://oeis.org/A130815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = [1, 5, 4, -1, -5, -4][n mod 6]
 * Range:     i8 n <= 71, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t table[6] = { (seq_t)1, (seq_t)5, (seq_t)4,
                                    (seq_t)-1, (seq_t)-5, (seq_t)-4 };
    return table[(size_t)(n % 6)];
}
