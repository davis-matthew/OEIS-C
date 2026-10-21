/*
 * A061044  Denominator of 1/25 - 1/n^2.
 * https://oeis.org/A061044
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of (1/25 - 1/n^2) in lowest terms
 * Range:     u8 n <= 5, u16 n <= 51, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute n^2 as seq_t to avoid overflow */
    seq_t nsq = (seq_t)n * (seq_t)n;

    /* Numerator: n^2 - 25      */
    seq_t num = nsq - (seq_t)25;

    /* Denominator before reduction: 25 * n^2 */
    seq_t den = (seq_t)25 * nsq;

    /* Reduce the fraction */
    seq_t g = gcd(num, den);
    return den / g;
}
