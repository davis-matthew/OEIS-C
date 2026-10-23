/*
 * A151648  Number of permutations of 5 indistinguishable copies of 1..n with
 *          exactly 3 adjacent element pairs in decreasing order.
 * https://oeis.org/A151648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 56^n - (5*n + 1)*21^n + binomial(5*n+1, 2)*6^n -
 *            binomial(5*n+1, 3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow, binom, mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binom.h"
#include "../utils/mul_small.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined for offset 1 but safe */

    seq_t nn = (seq_t)n;

    /* powers */
    seq_t pow56 = big_pow((seq_t)56, nn);
    seq_t pow21 = big_pow((seq_t)21, nn);
    seq_t pow6  = big_pow((seq_t)6, nn);

    /* (5*n+1) fits easily in an unsigned int */
    unsigned int coeff = (unsigned int)(5 * n + 1);

    /* terms of the formula */
    seq_t term2 = mul_small(pow21, coeff);
    seq_t term3 = seq_mul(binom(5 * n + 1, 2), pow6);
    seq_t term4 = binom(5 * n + 1, 3);

    /* compute result */
    seq_t res = pow56 - term2 + term3 - term4;
    return res;
}
