/*
 * A024019  a(n) = 2^n-n^9.
 * https://oeis.org/A024019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^n - n^9
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 10, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t power_two = big_pow((seq_t)2, (seq_t)n);
    /* Compute n^9 */
    seq_t power_n = big_pow((seq_t)n, (seq_t)9);
    /* Return difference */
    return power_two - power_n;
}
