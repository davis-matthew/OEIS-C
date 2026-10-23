/*
 * A151626  Number of permutations of 2 indistinguishable copies of 1..n with
 *          exactly 4 adjacent element pairs in decreasing order.
 * https://oeis.org/A151626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15^n - (2*n + 1)*10^n + C(2*n+1, 2)*6^n - C(2*n+1, 3)*3^n
 *            + C(2*n+1, 4)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Powers of the constants */
    seq_t pow15 = big_pow((seq_t)15, (seq_t)n);
    seq_t pow10 = big_pow((seq_t)10, (seq_t)n);
    seq_t pow6  = big_pow((seq_t)6,  (seq_t)n);
    seq_t pow3  = big_pow((seq_t)3,  (seq_t)n);

    /* Binomial coefficients */
    idx_t two_n_plus_one = n * 2 + 1;
    seq_t c2 = binom(two_n_plus_one, 2);
    seq_t c3 = binom(two_n_plus_one, 3);
    seq_t c4 = binom(two_n_plus_one, 4);

    /* Compute the expression term by term */
    seq_t result = pow15;

    seq_t temp = pow10 * ((seq_t)two_n_plus_one);
    result -= temp;

    temp = pow6 * c2;
    result += temp;

    temp = pow3 * c3;
    result -= temp;

    result += c4;

    return result;
}
