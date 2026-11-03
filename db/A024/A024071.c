/*
 * A024071  a(n) = 6^n - n^9.
 * https://oeis.org/A024071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6^n - n^9
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 11, i64 n <= 24, i128 n <= 49,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 497 (2026-11-01)
 * Helpers:   ipow, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t pow6 = ipow((seq_t)6, (seq_t)n);
    seq_t pow9 = ipow_idx((seq_t)n, 9);
    return pow6 - pow9;
}
