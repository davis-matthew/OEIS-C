/*
 * A024033  a(n) = 3^n - n^10.
 * https://oeis.org/A024033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 3^n - n^10
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 8, i64 n <= 39, i128 n <= 80,
 *            big any n
 * Helpers:   pow3, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t p3  = pow3(n);                 /* compute 3^n in seq_t */
    seq_t n10 = seq_pow((seq_t)n, (idx_t)10); /* compute n^10 in seq_t */
    return p3 - n10;                     /* return 3^n - n^10 */
}
