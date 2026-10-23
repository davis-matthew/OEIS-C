/*
 * A151630  Number of permutations of 2 indistinguishable copies of 1..n with
 *          exactly 8 adjacent element pairs in decreasing order.
 * https://oeis.org/A151630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..8} (-1)^j * binomial(2*n+1, j) * binomial(10-j,
 *            2)^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 7, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 2*n + 1 fits in idx_t for all practical n */
    idx_t two_n_plus_one = 2 * n + 1;

    seq_t pos = 0;   /* Sum of terms with even j */
    seq_t neg = 0;   /* Sum of terms with odd j  */

    for (int j = 0; j <= 8; ++j)
    {
        /* binomial(10-j, 2) is a small constant */
        seq_t base = binomial(10 - j, 2);

        /* base^n */
        seq_t pow = big_pow(base, (seq_t)n);

        /* binomial(2*n+1, j) */
        seq_t coeff = binomial(two_n_plus_one, j);

        /* term = coeff * pow */
        seq_t term = coeff * pow;

        if ((j & 1) == 0)
            pos += term;
        else
            neg += term;
    }

    /* pos >= neg, result is non‑negative */
    return pos - neg;
}
