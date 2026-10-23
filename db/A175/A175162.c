/*
 * A175162  a(n) = 16*(2^n + 1).
 * https://oeis.org/A175162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*(2^n + 1)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Computes the n-th term of OEIS sequence A175162.
   Returns 16*(2^n + 1) as a seq_t. */
seq_t a(idx_t n)
{
    seq_t result = pow2(n);   /* 2^n */
    result += 1;             /* 2^n + 1 */
    result *= 16;            /* 16*(2^n + 1) */
    return result;
}
