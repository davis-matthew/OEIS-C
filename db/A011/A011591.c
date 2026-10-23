/*
 * A011591  Legendre symbol (n,43).
 * https://oeis.org/A011591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,43)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(n) = 0 if n % 43 == 0, otherwise 1 if n is a quadratic residue mod 43,
   -1 if it is a non‑residue (Euler's criterion). */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* reduce n modulo 43 */
    idx_t r = n % 43;
    if (r == 0)
        return 0;

    /* apply Euler's criterion: n^21 ≡ 1 (mod 43) ⇔ n is a quadratic residue */
    seq_t mod = 43;
    seq_t pow = mod_pow((seq_t)r, 21, mod);

    return pow == 1 ? 1 : -1;
}
