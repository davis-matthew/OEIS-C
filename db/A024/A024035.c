/*
 * A024035  a(n) = 3^n - n^12.
 * https://oeis.org/A024035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 3^n - n^12
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 40, i128 n <= 80,
 *            big any n
 * Helpers:   pow3, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/pow_int.h"

/* Computes a(n) = 3^n - n^12 for any non‑negative integer n. */
seq_t a(idx_t n)
{
    /* 3^n  */
    seq_t p3 = pow3(n);
    /* n^12  */
    seq_t p12 = pow_int((seq_t)n, (idx_t)12);
    /* 3^n - n^12  */
    return p3 - p12;
}
