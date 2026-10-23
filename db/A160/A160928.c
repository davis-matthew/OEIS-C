/*
 * A160928  a(n) = n - reversal(n-1).
 * https://oeis.org/A160928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = n - rev(n-1)
 * Range:     i8 n <= 103, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/rev.h"

seq_t a(idx_t n)
{
    int64_t diff = (int64_t)n - (int64_t)rev(n - 1);
    return (seq_t)diff;
}
