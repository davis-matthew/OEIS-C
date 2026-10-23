/*
 * A138964  a(n) = 8^n mod 6^n.
 * https://oeis.org/A138964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8^n mod 6^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   mod_pow, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/mod_pow.h"

/* Computes 8^n modulo 6^n for n ≥ 0. */
seq_t a(idx_t n)
{
    seq_t mod = pow_uint(6, n);                 /* 6^n */
    return mod_pow((seq_t)8, n, mod);           /* 8^n mod 6^n */
}
