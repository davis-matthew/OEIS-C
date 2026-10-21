/*
 * A098107  Sum of all matrix elements M(i,j) = n!*(i/j), (i,j = 1..n).
 * https://oeis.org/A098107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n*(n+1)/2) * H(n) = (n*(n+1)/2) * Σ_{j=1..n} (n! /
 *            j) where H(n) = Σ_{j=1..n} 1/j
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset is 1; values below offset are irrelevant */

    /* compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* sum_{j=1..n} fact / j  (always integral) */
    seq_t sumDiv = 0;
    for (idx_t j = 1; j <= n; ++j)
        sumDiv += fact / (seq_t)j;

    /* triangular number T(n) = n*(n+1)/2 */
    seq_t triangular = (seq_t)n * ((seq_t)n + 1) / 2;

    /* final result */
    return triangular * sumDiv;
}
