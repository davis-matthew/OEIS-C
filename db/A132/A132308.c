/*
 * A132308  a(n) = 2*3^n - n - 1.
 * https://oeis.org/A132308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*3^n - n - 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t p3 = pow3(n);      /* compute 3^n */
    seq_t result = 2 * p3 - (seq_t)n - 1;
    return result;
}
