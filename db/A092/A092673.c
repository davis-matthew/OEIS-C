/*
 * A092673  a(n) = moebius(n) - moebius(n/2) where moebius(n) is zero if n is
 *          not an integer.
 * https://oeis.org/A092673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = μ(n) - μ(n/2) where μ is the Möbius function (μ(m) = 0 if
 *            m is not square‑free, otherwise μ(m) = (–1)^(number of prime
 *            factors). If n is odd, n/2 is not integer, so μ(n/2) is treated
 *            as 0
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* out of the defined domain, but safe */
    if (n & 1)                     /* odd n -> μ(n/2) contributes 0 */
        return mobius(n);
    else                            /* even n */
        return mobius(n) - mobius(n / 2);
}
