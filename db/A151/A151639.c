/*
 * A151639  Number of permutations of 3 indistinguishable copies of 1..n with
 *          exactly 9 adjacent element pairs in decreasing order.
 * https://oeis.org/A151639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..11} (-1)^(j+1)*binomial(3*n+1,
 *            11-j)*(binomial(j+1, 3))^n
 * Range:     u8 none, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static seq_t pow_seq_idx(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t result = 0;
    idx_t three_n_plus_one = 3 * n + 1;

    for (int j = 0; j <= 11; ++j)
    {
        int k = 11 - j;
        seq_t bin1 = binomial(three_n_plus_one, k);

        int bj = j + 1;
        seq_t bin2 = binomial(bj, 3);

        seq_t pow_term = pow_seq_idx(bin2, n);

        seq_t term = bin1 * pow_term;

        if ((j + 1) % 2 == 0)
            result += term;
        else
            result -= term;
    }

    return result;
}
