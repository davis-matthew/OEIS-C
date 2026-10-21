/*
 * A074113  Duplicate of A074116.
 * https://oeis.org/A074113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2^(3*n + floor((n-1)/3))
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed by the OEIS offset */
    idx_t e = 3*n + (n-1)/3;
    return pow2(e);
}
