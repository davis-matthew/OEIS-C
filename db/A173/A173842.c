/*
 * A173842  Number of permutations of 1..n with no adjacent pair summing to
 *          n+2.
 * https://oeis.org/A173842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..m} (-2)^j * Binomial(m, j) * (n - j)! , where m
 *            = max(0, floor((n - 1) / 2)). This implements the
 *            inclusion–exclusion formula from OEIS A173842
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* For uint64_t, if needed by the build. */

seq_t a(idx_t n)
{
    /* Handle the trivial case (also covered by the generic code). */
    if (n == 0) return (seq_t)1;

    /* Compute m = floor((n-1)/2). */
    idx_t m;
    if (n > 1) m = (n - 1) / 2;
    else m = 0;

    /* Compute factorial(n) iteratively. */
    seq_t f = (seq_t)1;          /* factorial(n) */
    for (idx_t i = 2; i <= n; ++i) {
        f *= (seq_t)i;
    }

    /* Initialize variables for the summation. */
    seq_t sum = (seq_t)0;                 /* Accumulated result */
    seq_t bin = (seq_t)1;                 /* Binomial(m, j) */
    seq_t p2  = (seq_t)1;                 /* 2^j */

    /* Inclusion–exclusion loop. */
    for (idx_t j = 0; j <= m; ++j) {
        /* term = bin * f * 2^j.  Sign handled by subtracting when j is odd. */
        seq_t term = bin * f;
        term *= p2;
        if ((j & 1) == 0) {
            sum += term;
        } else {
            sum -= term;
        }

        /* Prepare for next iteration. */
        if (j < m) {
            /* Update binomial: Binomial(m, j+1) = Binomial(m, j) * (m - j) / (j + 1). */
            bin *= (seq_t)(m - j);
            bin /= (seq_t)(j + 1);

            /* Double p2 for the next power. */
            p2 *= (seq_t)2;

            /* Reduce factorial for next j: (n - j - 1)! = (n - j)! / (n - j). */
            f /= (seq_t)(n - j);
        }
    }

    return sum;
}
