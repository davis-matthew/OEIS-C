/*
 * A053187  Square nearest to n.
 * https://oeis.org/A053187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(sqrt(n)+1/2)^2 (nearest square to n)
 * Range:     u8 n <= 31, u16 n <= 5883, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    /* Convert index to sequence type */
    seq_t nseq = (seq_t)n;

    /* Base case: when n = 0 */
    if (nseq == 0) return (seq_t)0;

    /* Find floor(sqrt(n)) */
    seq_t k = sqrt_floor(nseq);

    /* Lower and upper squares */
    seq_t low_sq = k * k;
    seq_t up     = k + 1;
    seq_t up_sq  = up * up;

    /* Distances to n */
    seq_t dist_low = nseq - low_sq;
    seq_t dist_up  = up_sq - nseq;

    return (dist_low <= dist_up) ? low_sq : up_sq;
}
