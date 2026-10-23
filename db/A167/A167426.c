/*
 * A167426  a(n) = 2^n mod 28.
 * https://oeis.org/A167426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 28
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Compute 2^n modulo 28.  mod_pow handles large exponents efficiently. */
    return mod_pow((seq_t)2, n, (seq_t)28);
}
