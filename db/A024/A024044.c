/*
 * A024044  a(n) = 4^n - n^8.
 * https://oeis.org/A024044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4^n - n^8
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 16, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 492 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* compute 4^n */
    seq_t pow4 = ipow((seq_t)4, (seq_t)n);
    /* compute n^8 by repeated multiplication */
    seq_t base = (seq_t)n;
    seq_t n8 = base;
    for (int i = 1; i < 8; ++i)
        n8 *= base;
    return pow4 - n8;
}
