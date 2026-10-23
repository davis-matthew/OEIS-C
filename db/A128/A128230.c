/*
 * A128230  Expansion of exp(x)/(1 - x - x^2/2!), where a(n) = 1 + n*a(n-1) +
 *          C(n,2)*a(n-2).
 * https://oeis.org/A128230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + n*a(n-1) + C(n,2)*a(n-2), a(0)=1, a(1)=2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)2;   /* a(n-1) */
    seq_t term;

    for (idx_t i = 2; i <= n; ++i) {
        /* term = 1 + i*prev1 + C(i,2)*prev2 */
        term = (seq_t)1 + ((seq_t)i) * prev1;

        /* compute C(i,2) = i*(i-1)/2 safely */
        seq_t binom;
        if ((i & 1) == 0) {
            binom = ((seq_t)(i/2)) * ((seq_t)(i-1));
        } else {
            binom = ((seq_t)i) * ((seq_t)((i-1)/2));
        }
        term += binom * prev2;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = term;
    }

    return prev1;
}
