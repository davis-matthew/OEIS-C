/*
 * A061690  Generalized Stirling numbers.
 * https://oeis.org/A061690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n! / (∑???)) -- derived from the generating function
 *            a(n) = (n!)^2/6 * Σ_{i+j+k=n, i,j,k≥1} (1/(i! j! k!))^2
 *            Simplification leads to: a(n) = ( Σ_{j=2}^{n-1} C(n,j)^2 *
 *            C(2j,j) - 2*(C(2n,n) - 2 - n^2) ) / 6 where C(m,r) is the
 *            binomial coefficient
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  // required by the build

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t sum = 0;                     // Σ C(n,j)^2 * C(2j,j) for j=2..n-1
    seq_t binom_n_j = 1;               // C(n,0)
    seq_t C_j_prev = 1;                // C_0 = 1
    seq_t C_j_current = 1;             // will hold C(j)

    for (idx_t j = 1; j <= n-1; ++j)
    {
        /* compute C(n,j) */
        binom_n_j = binom_n_j * (n - j + 1) / j;

        /* compute central binomial C(2j,j) */
        if (j == 1)
        {
            C_j_current = (seq_t)2;
        }
        else
        {
            /* C_j_current = C_j_prev * 2*(2*j-1) / j */
            C_j_current = C_j_prev *
                ( (seq_t)2 * ( (seq_t)2 * j - 1 ) ) / j;
        }

        /* accumulate for j ≥ 2 */
        if (j >= 2)
        {
            seq_t prod = binom_n_j * binom_n_j;
            prod *= C_j_current;
            sum += prod;
        }

        C_j_prev = C_j_current;
    }

    /* compute C(2n,n) = C_n = C_{n-1} * 2*(2n-1) / n */
    seq_t C_n = C_j_prev *
        ( (seq_t)2 * ( (seq_t)2 * n - 1 ) ) / n;

    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t T0 = C_n - (seq_t)2 - n_sq;

    seq_t numerator = sum - (T0 * (seq_t)2);
    seq_t result = numerator / (seq_t)6;

    return result;
}
