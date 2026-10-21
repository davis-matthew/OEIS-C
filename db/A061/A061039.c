/*
 * A061039  Numerator of 1/9 - 1/n^2.
 * https://oeis.org/A061039
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, frac, nice, easy
 * Formula:   a(n) = numerator of (1/9) - (1/n^2), i.e. a(n) = (n^2 - 9) /
 *            gcd(n^2 - 9, 9 * n^2) for n ≥ 3
 * Range:     u8 n <= 5, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Ensure we handle the defined offset; for n < 3 the value
       is not part of the sequence.  Return 0 for safety. */
    if (n < 3) return 0;

    /* Work entirely in seq_t; cast index first before any multiplication. */
    seq_t nsq = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t num = nsq - 9;                      /* numerator before reduction */
    seq_t den = (seq_t)9 * nsq;               /* full denominator 9*n^2 */

    seq_t g = gcd(num, den);                  /* greatest common divisor */
    return num / g;                           /* reduced numerator */
}
