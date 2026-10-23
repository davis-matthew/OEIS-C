/*
 * A134083  A007318 * A134082.
 * https://oeis.org/A134083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(r,k) * (2*(r-k)+1) where r is the unique integer with
 *            r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 and k = n - r*(r+1)/2
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom_choose
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binom_choose.h"

seq_t a(idx_t n)
{
    /* Determine the row r and column k from the linear index n */
    long double approx = sqrtl((long double)8 * (long double)n + 1.0L);
    idx_t r = (idx_t)((approx - 1.0L) / 2.0L);

    /* Correct rounding errors */
    while ((unsigned __int128)r * (r + 1) / 2 > n) --r;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) ++r;

    unsigned __int128 start128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t start = (idx_t)start128;
    idx_t k = n - start;          /* column index */

    /* Compute the value */
    seq_t binom = binom_choose(r, k);
    seq_t odd   = (seq_t)(2 * (seq_t)(r - k) + 1);
    return binom * odd;
}
