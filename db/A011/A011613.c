/*
 * A011613  Legendre symbol (n,151).
 * https://oeis.org/A011613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,151) = n^75 (mod 151), mapped to the
 *            integers -1, 0 and 1
 * Range:     i8 n <= 1, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Quick construction of the Legendre symbol (n,151). */
    /* If 151 divides n, then a(n)=0. Otherwise a(n) is +/- 1. */
    if (n % (idx_t)151 == 0)
        return 0;

    /* Compute n^75 mod 151.  mod_pow is defined in the utils. */
    seq_t mod = 151;
    seq_t r = mod_pow((seq_t)n, 75, mod);

    /* Map the modular result to the signed Legendre symbol. */
    if (r == 1)
        return 1;      /* quadratic residue */
    /* For a non‑zero non‑residue, r will be 150 (i.e., -1 mod 151). */
    return -1;
}
