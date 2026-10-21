/*
 * A066601  a(n) = 3^n mod n.
 * https://oeis.org/A066601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 3^n mod n
 * Range:     u8 n <= 328, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

/* Compute the n-th term of A066601: 3ⁿ modulo n (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Convert the base 3 and the modulus n to seq_t.  The exponent
       is already an idx_t.  The utility function mod_pow implements
       modular exponentiation by squaring and works for any seq_t
       size used by the build. */
    return mod_pow((seq_t)3, n, (seq_t)n);
}
