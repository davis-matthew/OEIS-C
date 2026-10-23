/*
 * A151636  Number of permutations of 3 indistinguishable copies of 1..n with
 *          exactly 6 adjacent element pairs in decreasing order.
 * https://oeis.org/A151636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..8} (-1)^j*binomial(3*n+1, 8-j)*(binomial(j+1,
 *            3))^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;
    for (idx_t j = 2; j <= 8; ++j) {
        idx_t k = 8 - j;
        seq_t binom_coeff = binomial(3 * n + 1, k);
        if (binom_coeff == 0) continue;
        /* binomial(j+1, 3)  = (j+1)*j*(j-1)/6  for j >= 2 */
        idx_t base_i = ((idx_t)(j + 1) * j * (j - 1)) / 6;
        if (base_i == 0) continue;
        seq_t base = (seq_t)base_i;
        seq_t pow_val = big_pow(base, (seq_t)n);
        seq_t term = binom_coeff * pow_val;
        if ((j & 1) == 0) {         /* even j: (+) */
            sum_pos += term;
        } else {                    /* odd j: (-) */
            sum_neg += term;
        }
    }
    return sum_pos - sum_neg;
}
