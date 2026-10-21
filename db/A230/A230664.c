/*
 * A230664  a(n) = floor(3^n/n^2).
 * https://oeis.org/A230664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(3^n / n^2)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Assumes n >= 1. Undefined behavior for n == 0. */
    seq_t numerator   = pow3(n);                  /* 3^n                     */
    seq_t denominator = ((seq_t)n) * ((seq_t)n);   /* n^2                     */
    return numerator / denominator;                /* floor division          */
}
