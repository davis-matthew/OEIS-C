/*
 * A011622  Legendre symbol (n,197).
 * https://oeis.org/A011622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,197) = n^98 (mod 197)
 * Range:     i8 n <= 1, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -a(n-1) - a(n-2) - … - a(n-196) (period 197) */
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    const seq_t MOD = 197;
    const uint64_t EXP = 98U;
    seq_t val = mod_pow((seq_t)(n % 197), EXP, MOD);   /* n^98 mod 197 */
    if (val == 0) return 0;
    return (val == 1) ? 1 : -1;                        /* 1 => residue, 196 => non‑residue */
}
