/*
 * A199764  a(n) = 10*11^n+1.
 * https://oeis.org/A199764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*11^n+1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Compute 11^n using fast exponentiation. */
    seq_t pow = pow_int((seq_t)11, n);

    /* Multiply by 10 and add 1. */
    return pow * (seq_t)10 + 1;
}
