/*
 * A024110  a(n) = 9^n - n^9.
 * https://oeis.org/A024110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 9^n - n^9
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 9, i64 n <= 19, i128 n <= 40,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 293 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t pow9n = ipow((seq_t)9, (seq_t)n);    /* 9^n   */
    seq_t pown9 = ipow((seq_t)n, (seq_t)9);    /* n^9   */
    return pow9n - pown9;
}
