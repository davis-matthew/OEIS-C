/*
 * A151649  Number of permutations of 5 indistinguishable copies of 1..n with
 *          exactly 4 adjacent element pairs in decreasing order.
 * https://oeis.org/A151649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..4} (-1)^j * binomial(5*n+1, j) * binomial(9-j,
 *            5)^n (OEIS A151649)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* binomial(9-j,5) for j = 0..4 */
static const seq_t const_k[5] = { (seq_t)126, (seq_t)56, (seq_t)21, (seq_t)6, (seq_t)1 };

/* Computes a(n) for n >= 1 */
seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t main = 5 * n + 1;          /* 5*n+1  (idx_t is uint64_t)          */
    seq_t big_n = (seq_t)n;          /* exponent for big_pow                 */

    for (idx_t j = 0; j < 5; ++j) {
        /* binomial(main, j) */
        seq_t comb = binomial(main, j);

        /* const_k[j]^n */
        seq_t power = big_pow(const_k[j], big_n);

        /* term = comb * power */
        seq_t term = comb * power;

        /* add/subtract based on parity of j */
        if ((j & 1) == 0) {
            result += term;
        } else {
            result -= term;
        }
    }
    return result;
}
