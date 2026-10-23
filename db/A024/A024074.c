/*
 * A024074  a(n) = 6^n - n^12.
 * https://oeis.org/A024074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6^n - n^12
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 24, i128 n <= 49,
 *            big any n
 * Helpers:   big_pow, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* 6^n */
    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);
    
    /* n^12 */
    seq_t pown12 = pow_int((seq_t)n, 12);
    
    return pow6 - pown12;
}
