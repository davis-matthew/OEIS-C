/*
 * A204042  The number of functions f:{1,2,...,n}->{1,2,...,n} (endofunctions)
 *          such that all of the fixed points in f are isolated.
 * https://oeis.org/A204042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} (j-1)^j * binomial(n,j)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t binom = 1;               /* C(n,0) */

    for (idx_t j = 0; j <= n; ++j) {
        seq_t pow_term;

        if (j == 0) {
            pow_term = 1;          /* (-1)^0 = 1 */
        } else if (j == 1) {
            pow_term = 0;          /* 0^1 = 0 */
        } else {
            pow_term = big_pow((seq_t)(j - 1), (seq_t)j);
        }

        result += binom * pow_term;

        if (j == n) break;        /* finished last term */

        /* Update binomial coefficient for next j:
           C(n, j+1) = C(n, j) * (n-j) / (j+1) */
        binom = binom * ((seq_t)(n - j)) / ((seq_t)(j + 1));
    }

    return result;
}
