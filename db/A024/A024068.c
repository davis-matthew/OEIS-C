/*
 * A024068  a(n) = 6^n - n^6.
 * https://oeis.org/A024068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6^n - n^6
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 11, i64 n <= 24, i128 n <= 49,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 181 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t pow6 = ipow((seq_t)6, (seq_t)n);
    seq_t n6   = ipow((seq_t)n, (seq_t)6);
    return pow6 - n6;
}
