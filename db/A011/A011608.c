/*
 * A011608  Legendre symbol (n,127).
 * https://oeis.org/A011608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = n^63 (mod 127)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Reduce n modulo 127; 127 is prime, so the Legendre symbol is
       the quadratic residue test modulo 127.  The exponent (p-1)/2 = 63
       gives 1 for residues, 126 (= -1 mod 127) for non-residues, and
       0 when n ≡ 0 (mod 127). */
    seq_t base = (seq_t)(n % 127);
    seq_t r = mod_pow(base, 63, 127);

    if (r == 0)          return 0;  /* n ≡ 0 (mod 127) */
    if (r == 1)          return 1;  /* quadratic residue */
    /* r == 126: -1 mod 127 */
    return -1;
}
