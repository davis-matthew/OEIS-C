/*
 * A011607  Legendre symbol (n,113).
 * https://oeis.org/A011607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = n^56 mod 113, interpreted as the Legendre symbol (n,113)
 *            = 0 if n ≡ 0 (mod 113), +1 if n is a quadratic residue mod 113,
 *            -1 otherwise
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t MOD = 113ULL;
    uint64_t residue = (uint64_t)n % MOD;   /* n modulo 113 */
    uint64_t result = 1;
    for (unsigned i = 0; i < 56; ++i)
        result = (result * residue) % MOD;

    if (result == 0)          return (seq_t)0;      /* n divisible by 113 */
    if (result == MOD - 1)    return (seq_t)(-1);  /* quadratic non‑residue */
    return (seq_t)1;          /* quadratic residue */
}
