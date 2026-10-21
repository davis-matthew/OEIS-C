/*
 * A088957  Hyperbinomial transform of the sequence of 1's.
 * https://oeis.org/A088957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k=0..n} (n-k+1)^(n-k-1) * C(n, k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t j = 0; j <= n; ++j) {          /* j = n - k   */
        idx_t k = n - j;                      /* k = n - j   */
        seq_t comb = binomial(n, k);          /* C(n, k)     */
        seq_t pow_term;
        if (j == 0) {                         /* (1)^(-1) defined as 1  */
            pow_term = (seq_t)1;
        } else {
            seq_t base   = (seq_t)(j + 1);    /* (j+1)         */
            seq_t exp    = (seq_t)(j - 1);    /* (j-1)         */
            pow_term = big_pow(base, exp);   /* (j+1)^(j-1)   */
        }
        sum += pow_term * comb;
    }
    return sum;
}
