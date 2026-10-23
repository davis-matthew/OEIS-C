/*
 * A011604  Legendre symbol (n,103).
 * https://oeis.org/A011604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) == n^51 (mod 103)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    const uint64_t P = 103;          /* prime modulus */
    seq_t r = mod_pow((seq_t)(n % P), 51, (seq_t)P);  /* Euler criterion */
    if (r == 0) return 0;           /* n divisible by 103 */
    if (r == 1) return 1;           /* quadratic residue */
    return -1;                      /* quadratic non-residue (r == 102) */
}
