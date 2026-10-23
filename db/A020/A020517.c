/*
 * A020517  9th cyclotomic polynomial evaluated at powers of 2.
 * https://oeis.org/A020517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 8^n + 64^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for the 9th cyclotomic polynomial evaluated at powers of 2. */
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute 8^n and 64^n with arbitrary-precision power */
    seq_t pow8  = seq_pow((seq_t)8,  n);
    seq_t pow64 = seq_pow((seq_t)64, n);
    return (seq_t)1 + pow8 + pow64;
}
