/*
 * A211513  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          2*w^2=x*y.
 * https://oeis.org/A211513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (w,x,y) with 1 ≤ w,x,y ≤ n
 *            satisfying 2*w^2 = x*y. Computed by iterating all (x,y) pairs
 * Range:     u8 n <= 11, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t x = 1; x <= n; ++x) {
        for (idx_t y = 1; y <= n; ++y) {
            seq_t prod = (seq_t)x * y;          /* x*y fits in seq_t */
            if ((prod & 1) != 0)                /* product must be even */
                continue;
            seq_t half = prod >> 1;             /* (x*y)/2 */
            seq_t w = floor_sqrt(half);         /* integer sqrt of half */
            if (w * w == half)                  /* check perfect square */
                ++count;                         /* found a valid triple */
        }
    }
    return count;
}
