/*
 * A024136  a(n) = 11^n - n^9.
 * https://oeis.org/A024136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 11^n - n^9
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 9, i64 n <= 18, i128 n <= 36,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 257 (2026-11-01)
 * Helpers:   ipow, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t pow11 = ipow((seq_t)11, (seq_t)n);
    seq_t pow9  = ipow_u64_idx((uint64_t)n, 9);
    return pow11 - pow9;
}
