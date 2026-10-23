/*
 * A122039  Duplicate of A083706.
 * https://oeis.org/A122039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2^n + n - 2
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t val = pow2(n);   /* compute 2^n */
    val += (seq_t)n;       /* add n */
    val -= 2;              /* subtract 2 */
    return val;
}
