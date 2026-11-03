/*
 * A024150  a(n) = 12^n - n^10.
 * https://oeis.org/A024150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 12^n - n^10
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 9, i64 n <= 17, i128 n <= 35,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 277 (2026-11-01)
 * Helpers:   ipow, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t pow12 = ipow((seq_t)12, (seq_t)n);
    seq_t pow10 = ipow_idx((seq_t)n, 10);
    return pow12 - pow10;
}
