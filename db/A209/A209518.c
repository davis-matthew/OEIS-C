/*
 * A209518  Triangle by rows, reversal of A104712.
 * https://oeis.org/A209518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial( r + 2 , k ), where r = floor( (sqrt(8*n + 1) -
 *            1) / 2 ), // row index k = n - r*(r + 1)/2. The triangle is the
 *            first r+1 binomial coefficients of order r+2
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"

/* Public function required by the judge */
seq_t a(idx_t n)
{
    /* Compute the row r that contains the index n */
    long double d = sqrtl((long double)(8.0L * (long double)n + 1.0L));
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);

    /* Column index within the row */
    idx_t k = n - r * (r + 1) / 2;

    /* Return the binomial coefficient C(r+2, k) */
    return binomial((idx_t)r + 2, (idx_t)k);
}
