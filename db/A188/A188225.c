/*
 * A188225  Number of ways to select 15 knights from n knights sitting at a
 *          round table if no adjacent knights are chosen.
 * https://oeis.org/A188225
 *
 * Offset:    30    (a(30) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n - 15, 15) + C(n - 16, 14)
 * Range:     u8 n <= 31, u16 n <= 34, u32 n <= 45, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 30)
        return 0;                                      /* below offset, value is undefined */

    seq_t term1 = binomial(n - 15, 15);                 /* C(n-15, 15) */
    seq_t term2 = binomial(n - 16, 14);                 /* C(n-16, 14) */

    return term1 + term2;
}
