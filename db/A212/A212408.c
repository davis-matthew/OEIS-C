/*
 * A212408  Number of binary arrays of length 2*n+6 with no more than n ones
 *          in any length 2n subsequence (=50% duty cycle).
 * https://oeis.org/A212408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n+5) - 4*(955*n^3 - 3782*n^2 + 3475*n + 30) *
 *            C(2*n-7, n-4) / ((n-2)*(n-1)*n) for n ≥ 4 a(1)=55, a(2)=285,
 *            a(3)=1314
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   binom, gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* small explicit values */
    if (n == 1) return (seq_t)55;
    if (n == 2) return (seq_t)285;
    if (n == 3) return (seq_t)1314;

    seq_t N = (seq_t)n;

    /* polynomial part: 955*n^3 - 3782*n^2 + 3475*n + 30 */
    seq_t n2   = N * N;          /* n^2 */
    seq_t n3   = n2 * N;         /* n^3 */
    seq_t poly = 955 * n3 - 3782 * n2 + 3475 * N + 30;

    /* binomial coefficient C(2*n-7, n-4) */
    seq_t bin = binom(2*n - 7, n - 4);

    /* denominator (n-2)*(n-1)*n */
    seq_t denom = (N - 2) * (N - 1) * N;

    /* term = 4 * poly * bin / denom, computed safely to avoid overflow */
    seq_t term = bin;
    term *= 4;                     /* term = bin * 4 */

    seq_t g = gcd(term, denom);    /* reduce first factor */
    term /= g;
    denom /= g;

    term *= poly;                  /* term = term * poly */
    term /= denom;                 /* final division; exact */

    seq_t pow = pow2(2*n + 5);      /* 2^(2*n+5) */

    return pow - term;
}
