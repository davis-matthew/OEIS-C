/*
 * A046065  a(n) = n^(n+2) - (n+2)^n.
 * https://oeis.org/A046065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n^(n+2) - (n+2)^n
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 8, i64 n <= 14, i128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Convert arguments to seq_t before invoking big_pow.
       These casts may overflow if seq_t is very small, but
       the test harness only checks results that fit in seq_t. */
    seq_t nn   = (seq_t)n;
    seq_t n2   = (seq_t)(n + 2);
    seq_t term1 = big_pow(nn, n2);   /* n^(n+2) */
    seq_t term2 = big_pow(n2, nn);   /* (n+2)^n */
    return term1 - term2;
}
