/*
 * A047863  Number of labeled graphs with 2-colored nodes where black nodes
 *          are only connected to white nodes and vice versa.
 * https://oeis.org/A047863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{k=0..n} binomial(n, k) * 2^(k*(n-k))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom = 1;

    for (idx_t k = 0; k <= n; ++k) {
        /* exponent k*(n-k) fits in idx_t for all practical n */
        idx_t exp = k * (n - k);
        seq_t pow_term = pow2(exp);

        sum += binom * pow_term;

        /* update binomial coefficient for next k */
        if (k < n) {
            binom = binom * (n - k) / ((seq_t)(k + 1));
        }
    }
    return sum;
}
