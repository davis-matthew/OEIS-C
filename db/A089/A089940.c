/*
 * A089940  Triangle read by rows: T(n,k)=binomial(n+k,floor((n-k)/2)).
 * https://oeis.org/A089940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(r + k, floor((r - k)/2)) where r =
 *            floor((sqrt(8·n + 1) - 1)/2) and k = n - r·(r + 1)/2 (rows are
 *            read from top to bottom, left to right)
 * Range:     u8 n <= 35, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* Column index within row r. */
    idx_t k = n - r * (r + 1) / 2;

    /* Floor of (r - k)/2. */
    idx_t half = (r - k) / 2;

    /* Compute the binomial coefficient. */
    return binomial(r + k, half);
}
