/*
 * A024121  a(n) = 10^n - n^7.
 * https://oeis.org/A024121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 10^n - n^7
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 9, i64 n <= 18, i128 n <= 38,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 292 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    seq_t ten_pow = ipow_idx((seq_t)10, n);               /* 10^n   */
    seq_t n_pow7  = ipow_idx((seq_t)n, (idx_t)7);          /* n^7    */
    return ten_pow - n_pow7;
}
