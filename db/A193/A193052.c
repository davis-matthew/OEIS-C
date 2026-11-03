/*
 * A193052  a(n) = 5^n - 4^n - 3^n.
 * https://oeis.org/A193052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5^n - 4^n - 3^n
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 27, i128 n <= 54,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 478 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t pow5 = ipow((seq_t)5, (seq_t)n);
    seq_t pow4 = ipow((seq_t)4, (seq_t)n);
    seq_t pow3 = ipow((seq_t)3, (seq_t)n);
    return pow5 - pow4 - pow3;
}
