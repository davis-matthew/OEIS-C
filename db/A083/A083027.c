/*
 * A083027  Duplicate of A060107.
 * https://oeis.org/A083027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) is the nth non‑negative integer that is congruent to 0, 2,
 *            3, 5, 7, 8 or 10 modulo 12. Equivalently, a(n) = 12 * floor(n /
 *            7) + offset[n % 7] with offset = {0, 2, 3, 5, 7, 8, 10}
 * Range:     i8 n <= 24, i16 n <= 24, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t offset[7] = {0, 2, 3, 5, 7, 8, 10};
    idx_t q = n / 7;
    size_t r = n % 7;
    return (seq_t)q * 12 + offset[r];
}
