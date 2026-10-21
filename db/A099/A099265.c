/*
 * A099265  Partial sums of A056272.
 * https://oeis.org/A099265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5^n + 12*3^n + 32*2^n + 36*n - 45)/96
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the explicit expression for A099265 with c = 5. */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute powers using the helper pow_uint (returns seq_t). */
    seq_t p5 = pow_uint(5, n);
    seq_t p3 = pow_uint(3, n);
    seq_t p2 = pow_uint(2, n);

    /* Compute numerator: 5^n + 12*3^n + 32*2^n + 36*n - 45. */
    seq_t num = p5
              + (seq_t)12 * p3
              + (seq_t)32 * p2
              + (seq_t)36 * ((seq_t)n)
              - (seq_t)45;

    /* Divide by 96 (the LCM of the denominators). */
    return num / (seq_t)96;
}
