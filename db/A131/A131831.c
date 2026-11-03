/*
 * A131831  Duplicate of A100314.
 * https://oeis.org/A131831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2^n + 2*n
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 26, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t two_n = ((seq_t)n) * 2;
    return pow2(n) + two_n;
}
