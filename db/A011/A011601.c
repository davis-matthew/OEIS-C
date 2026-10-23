/*
 * A011601  Legendre symbol (n,89).
 * https://oeis.org/A011601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,89)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    const seq_t MOD = 89;
    /* If n is a multiple of 89, the Legendre symbol is 0. */
    seq_t r = (seq_t)(n % 89);
    if (r == 0)
        return 0;

    /* n^{44} (mod 89) gives 1 for residues, 88 for non‑residues. */
    seq_t rem = mod_pow(r, 44, MOD);

    return (rem == 1) ? 1 : -1;
}
