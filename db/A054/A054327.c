/*
 * A054327  Ninth column of Lanczos triangle A053125 (decreasing powers).
 * https://oeis.org/A054327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n * binomial(2*n+9, 8)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 18, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* compute 4^n */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow4 *= (seq_t)4;
    }

    /* compute binomial(2*n+9, 8) multiplicatively */
    seq_t binom = 1;
    idx_t num_base = 2 * n + (idx_t)9;   /* n in the binomial is 2*n+9 */
    for (idx_t k = 1; k <= (idx_t)8; ++k) {
        seq_t numerator = (seq_t)(num_base - (k - 1));
        binom = binom * numerator / (k);
    }

    return pow4 * binom;
}
