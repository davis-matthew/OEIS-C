/*
 * A024072  a(n) = 6^n - n^10.
 * https://oeis.org/A024072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6^n - n^10
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 8, i64 n <= 24, i128 n <= 49,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 488 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

/* External linkage function that computes the OEIS sequence A024072 */
seq_t a(idx_t n)
{
    /* Compute 6^n and n^10 using the provided big_pow utility */
    seq_t pow6  = ipow((seq_t)6,   (seq_t)n);
    seq_t pow10 = ipow((seq_t)n,   (seq_t)10);
    /* Return the difference */
    return pow6 - pow10;
}
