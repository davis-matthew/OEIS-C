/*
 * A024020  a(n) = 2^n - n^10.
 * https://oeis.org/A024020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2^n - n^10
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 8, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 215 (2026-11-01)
 * Helpers:   ipow_idx, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/ipow_idx.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t two_pow = pow2(n);                  /* 2^n  */
    seq_t n_pow10 = ipow_idx((seq_t)n, 10);    /* n^10 */
    return two_pow - n_pow10;
}
