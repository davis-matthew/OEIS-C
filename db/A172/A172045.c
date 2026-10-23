/*
 * A172045  a(n) = (9*n^4+10*n^3-3*n^2-4*n)/12.
 * https://oeis.org/A172045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(9*n^2+n-4)/12
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 274, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* convert the index to seq_t for all arithmetic */
    seq_t nn = (seq_t)n;

    /* the three factors of the numerator */
    seq_t f0 = nn;                                /* n */
    seq_t f1 = nn + (seq_t)1;                     /* n+1 */
    seq_t f2 = (seq_t)9 * nn * nn + nn - (seq_t)4;/* 9*n^2 + n - 4 */

    /* denominator 12, will be reduced step by step */
    seq_t denom = (seq_t)12;
    seq_t result = (seq_t)1;

    /* reduce f0 with the current denominator */
    seq_t g = gcd(f0, denom);
    if (g != (seq_t)1) { f0 /= g; denom /= g; }
    result *= f0;

    /* reduce f1 */
    g = gcd(f1, denom);
    if (g != (seq_t)1) { f1 /= g; denom /= g; }
    result *= f1;

    /* reduce f2 */
    g = gcd(f2, denom);
    if (g != (seq_t)1) { f2 /= g; denom /= g; }
    result *= f2;

    /* if anything remains in the denominator divide it out */
    if (denom != (seq_t)1) result /= denom;

    return result;
}
