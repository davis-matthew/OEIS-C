/*
 * A056986  Number of permutations on {1,...,n} containing any given pattern
 *          alpha in the symmetric group S_3.
 * https://oeis.org/A056986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! - C_n, where C_n is the nth Catalan number (C_n =
 *            binomial(2n,n)/(n+1)). For n < 3 the result is 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n < 3)
        return (seq_t)0;

    /* Compute n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* Compute binomial(2n, n) = product_{i=1}^{n} (n+i)/i */
    seq_t binom = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        binom *= (seq_t)(n + i);
        binom /= (seq_t)i;        /* exact division */
    }

    /* Catalan number: C_n = binom / (n+1) */
    seq_t cat = binom / (seq_t)(n + 1);

    return fact - cat;
}
