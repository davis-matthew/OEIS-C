/*
 * A174247  Number of words of length n composed of the letters a, b, and c
 *          with at least as many a's as b's, and at least as many b's as c's,
 *          and no a's adjacent.
 * https://oeis.org/A174247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=floor((n+2)/3)}^{floor((n+1)/2)} *
 *            Sum_{j=floor((n-i+1)/2)}^{min(n-i, i)} * binomial(n-i+1, i) *
 *            binomial(n-i, j)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 51, u128 n <= 100,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The binomial helper is provided by the build system; include its
   header to make the function visible. */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* i ranges from floor((n+2)/3) to floor((n+1)/2) */
    idx_t i_min = (n + 2) / 3;
    idx_t i_max = (n + 1) / 2;

    for (idx_t i = i_min; i <= i_max; ++i) {
        /* binomial(n-i+1, i) – number of ways to place the a's */
        seq_t binomA = binom(n - i + 1, i);

        idx_t m = n - i;                 /* remaining positions */
        idx_t L = (m + 1) / 2;           /* lower bound for j */
        idx_t U = (m < i) ? m : i;       /* upper bound for j */

        seq_t inner_sum = 0;
        seq_t b = 1;                     /* current binomial(m, j) */

        for (idx_t j = 0; j <= U; ++j) {
            if (j >= L) {
                inner_sum += b;
            }
            if (j < U) { /* compute binomial(m, j+1) */
                b = (b * (seq_t)(m - j)) / (seq_t)(j + 1);
            }
        }

        result += binomA * inner_sum;
    }

    return result;
}
