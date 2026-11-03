/*
 * A024147  a(n) = 12^n - n^7.
 * https://oeis.org/A024147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 12^n - n^7
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 17, i128 n <= 35,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 245 (2026-11-01)
 * Helpers:   ipow_i32, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t pw12 = ipow_i32((seq_t)12, (int)n);
    seq_t pwn  = ipow_idx((seq_t)n, 7);
    return pw12 - pwn;
}
