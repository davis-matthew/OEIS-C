/*
 * A159863  a(3)(baseN) = least number m such that the quotient m/3 is
 *          obtained merely by shifting the leftmost digit (MSD) of m to the
 *          right end (LSD), and the multiple 3m by shifting the LSD of m to
 *          the MSD, in order of increasing number base, N.
 * https://oeis.org/A159863
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 5, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   digits_as_decimal, gcd, minimal_m_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A159863 (OEIS)
 *
 *  a(n) is the decimal integer obtained by writing the least base-n number
 *  m that satisfies
 *
 *     m / 3   = rotate_left(m)   (MSD moved to LSD)
 *     3 * m   = rotate_right(m)  (LSD moved to MSD)
 *
 *  The function returns the decimal representation of the base‑n digits of
 *  that number.  For n < 4 it returns 0.  The algorithm searches the
 *  digit length k, computes the base‑N power pow = N^(k-1), and tests
 *  every possible most significant digit a (1 … N-1) for the
 *  Diophantine relation
 *
 *          a * (pow - 3)  is divisible by 3*N - 1
 *          t = a * (pow - 3) / (3*N - 1)  < pow
 *
 *  where t is the remaining lower k-1 digits.  The two shift
 *  conditions are then verified explicitly.  The first such m found
 *  (i.e. the one with the smallest k) is returned.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/gcd.h"   /* provides gcd(seq_t, seq_t) */
#include "../utils/digits_as_decimal.h"
#include "../utils/minimal_m_decimal.h"

/* External function required by the build */
seq_t a(idx_t n)
{
    return minimal_m_decimal(n);
}
