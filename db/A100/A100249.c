/*
 * A100249  Antidiagonal sums of the slanted Catalan convolution table
 *          A100247.
 * https://oeis.org/A100249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊2n/3⌋} C(n+k-⌊k/2⌋, k) *
 *            (n-k-⌊k/2⌋)/(n+k-⌊k/2⌋)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Handle the offset: a(0) = 1 is defined separately */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;
    idx_t maxk = (2 * n) / 3;  /* floor(2n/3) */

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t half_k = k / 2;           /* floor(k/2) */
        idx_t N = n + k - half_k;       /* n + k - floor(k/2) */

        seq_t bin = binom(N, k);        /* C(N, k) */

        seq_t num = (seq_t)(n - k - half_k);     /* n - k - floor(k/2) */
        seq_t den = (seq_t)(n + k - half_k);     /* n + k - floor(k/2) */

        /* The division is exact by definition of the sequence */
        seq_t term = (bin * num) / den;

        sum += term;
    }

    return sum;
}
