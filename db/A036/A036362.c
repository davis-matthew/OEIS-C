/*
 * A036362  Number of labeled 3-trees with n nodes.
 * https://oeis.org/A036362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,3)*(3*n-8)^(n-5)
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 10, u64 n <= 15, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    if (n == 3 || n == 4)
        return 1;

    /* binomial(n,3) */
    seq_t bin = binomial(n, 3);

    /* base = 3*n - 8 */
    seq_t base = ((seq_t)3 * n) - 8;

    /* exponent = n - 5 */
    seq_t exp = (seq_t)(n - 5);

    /* (3*n-8)^(n-5) */
    seq_t pow_val = big_pow(base, exp);

    return bin * pow_val;
}
