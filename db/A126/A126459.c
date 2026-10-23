/*
 * A126459  Column 1 of triangle A126457; a(n) = C( C(n+3,3) + 2, n).
 * https://oeis.org/A126459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial( binomial(n+3,3)+2 , n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 6, a(2) = 66, ... (OEIS A126459) */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute m = C(n+3,3) + 2 */
    seq_t m = binomial(n + 3, 3) + 2;

    /* m is guaranteed to fit in idx_t for the n range of interest
       (up to about 10000), so cast safely for the outer binomial. */
    idx_t m_idx = (idx_t) m;

    /* Return C(m, n) */
    return binomial(m_idx, n);
}
