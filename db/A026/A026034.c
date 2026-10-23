/*
 * A026034  a(n) = A026022(n,floor(n/2)).
 * https://oeis.org/A026034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n, floor(n/2)) - binomial(n, floor(n/2)-4)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t k = n / 2;                  /* floor(n/2) */
    seq_t term1 = binomial(n, k);     /* C(n, k)            */
    seq_t term2 = 0;                  /* C(n, k-4) if k ≥ 4 */
    if (k >= 4)
        term2 = binomial(n, k - 4);
    return term1 - term2;
}
