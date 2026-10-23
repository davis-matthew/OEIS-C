/*
 * A036128  a(n) = 2^n mod 53.
 * https://oeis.org/A036128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 53
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes 2 raised to n modulo 53 using a fast modular exponentiation routine. */
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    return mod_pow((seq_t)2, n, (seq_t)53);
}
