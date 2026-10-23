/*
 * A187536  Partial sums of the central Lah numbers (A187535).
 * https://oeis.org/A187536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k=0..n} binomial(2k-1,k-1)*(2k)!/k!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)1;          /* a(0) = 1 */
    seq_t ratio = (seq_t)1;        /* ratio_k = (2k)!/k! */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update ratio: ratio_{k} = ratio_{k-1} * (2k-1) * (2k) / k */
        ratio = (ratio * (seq_t)(2*(idx_t)k - 1) * (seq_t)(2*(idx_t)k)) / (seq_t)k;

        /* Compute binomial(2k-1, k-1) */
        seq_t comb = binomial((idx_t)(2*k - 1), (idx_t)(k - 1));

        /* term = comb * ratio */
        seq_t term = comb * ratio;

        sum += term;
    }

    return sum;
}
