/*
 * A178182  Minimal polynomials of sin(2Pi/n) are mapped to those of
 *          cos(2Pi/a(n)).
 * https://oeis.org/A178182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n / gcd(|n-4|, 16)
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute the absolute value of n-4 without unsigned wrap‑around. */
    idx_t diffIdx = n > 4 ? n - 4 : 4 - n;
    seq_t diff = (seq_t)diffIdx;

    /* gcd for seq_t values. */
    seq_t g = gcd(diff, (seq_t)16);

    return ((seq_t)4 * (seq_t)n) / g;
}
