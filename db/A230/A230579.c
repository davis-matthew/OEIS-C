/*
 * A230579  a(n) = 2^n mod 341.
 * https://oeis.org/A230579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 341
 * Range:     u8 n <= 6, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute 2^n modulo 341 using the generic modular exponentiation routine. */
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Base 2, exponent n, modulus 341. */
    return mod_pow((seq_t)2, n, (seq_t)341);
}
