/*
 * A034670  Duplicate of A020516.
 * https://oeis.org/A034670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 1 + 2^n + 2^(2n) + 2^(3n) + 2^(4n) + 2^(5n) + 2^(6n)
 *            (equivalently, a(n) = (2^(7n)-1)/(2^n-1) for n>0, and a(0)=7)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 7, i128 n <= 7,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t base = seq_pow((seq_t)2, n);

    seq_t term   = 1;   /* current 2^(k*n) */
    seq_t result = 1;   /* accumulates the sum */

    for (idx_t i = 0; i < 6; ++i) {
        term *= base;   /* term = 2^((k+1)*n) */
        result += term;
    }

    return result;
}
