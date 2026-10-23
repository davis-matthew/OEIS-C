/*
 * A145216  Self-convolution of (1^3, 2^3, 3^3, 4^3, ... ).
 * https://oeis.org/A145216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+2,3) * (3*n*(n+2)*(n^2+2*n+3)+16) / 70
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 47, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* convert index to seq_t */
    seq_t N  = (seq_t)n;
    seq_t N1 = N + 1;
    seq_t N2 = N + 2;

    /* binomial part: C(n+2,3) = n*(n+1)*(n+2)/6 */
    seq_t binom = N * N1 * N2 / 6;

    /* auxiliary term: n^2 + 2*n + 3 */
    seq_t u = N * N + 2 * N + 3;

    /* inner factor: 3*n*(n+2)*u + 16 */
    seq_t temp = 3 * N * N2 * u + 16;

    /* reduce to avoid overflow for small builds */
    seq_t denom = 70;

    seq_t g = gcd(binom, denom);
    binom /= g;
    denom /= g;

    g = gcd(temp, denom);
    temp /= g;
    denom /= g;

    /* denom should be 1; final exact division */
    return binom * temp / denom;
}
