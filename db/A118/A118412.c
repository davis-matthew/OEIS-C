/*
 * A118412  Denominator of sum of reciprocals of first n pentatope numbers
 *          A000332.
 * https://oeis.org/A118412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = reduced denominator of \sum_{i=1..n} 1 / C(i+3,4) =
 *            (4*(D-6)) / (3*D) where D = (n+1)(n+2)(n+3)
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 1502, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* n is 1-indexed. Work in seq_t to avoid overflow. */
    seq_t n1 = (seq_t)n + 1;
    seq_t n2 = (seq_t)n + 2;
    seq_t n3 = (seq_t)n + 3;

    seq_t D = n1 * n2 * n3;          /* (n+1)(n+2)(n+3)             */
    seq_t N = 4 * (D - 6);           /* 4*(D-6)                      */
    seq_t den = 3 * D;               /* 3*D                          */

    seq_t g = gcd(N, den);           /* reduce the fraction          */
    den /= g;
    return den;
}
