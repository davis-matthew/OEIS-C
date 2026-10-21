/*
 * A084946  Duplicate of A047055.
 * https://oeis.org/A084946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = product of (5*k - 3) for k = 1 .. n, with a(0) = 1
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 9, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k)
        result *= (seq_t)(5 * k - 3);
    return result;
}
