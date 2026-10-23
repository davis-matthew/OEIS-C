/*
 * A115053  Series expansion of x*(x+3)^2/(3*x+1)^2.
 * https://oeis.org/A115053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 0 for n = 0 9 for n = 1 -48 for n = 2 16 * (4*n + 1) *
 *            (-3)^(n-3) for n >= 3 (sequence A115053)
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 16, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 9;
    if (n == 2) return -48;

    /* Compute coefficient 16*(4*n+1) */
    seq_t coeff = 16 * (((seq_t)n * 4) + 1);

    /* Compute power (-3)^(n-3) */
    seq_t pow_part = big_pow((seq_t)-3, (seq_t)(n - 3));

    /* Final result */
    return coeff * pow_part;
}
