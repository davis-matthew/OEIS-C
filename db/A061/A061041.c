/*
 * A061041  Numerator of 1/16 - 1/n^2.
 * https://oeis.org/A061041
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, frac, nice, easy
 * Formula:   a(n) = (n^2 - 16) / gcd(16*n^2, n^2 - 16)
 * Range:     u8 none, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Return the nth term of OEIS A061041.
 * The sequence is defined for n ≥ 4; values for smaller n are undefined
 * but a fallback of 0 is provided for safety. */
seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;           /* defined only for n ≥ 4 */

    seq_t sq   = (seq_t)n * (seq_t)n;    /* n² */
    seq_t num  = sq - (seq_t)16;        /* numerator: n² – 16 */
    seq_t den  = (seq_t)16 * sq;        /* denominator: 16 n² */

    seq_t g    = gcd(num, den);         /* gcd using provided helper */
    return num / g;                     /* reduced numerator */
}
