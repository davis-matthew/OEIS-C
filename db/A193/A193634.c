/*
 * A193634  Triangle: T(n,k)=C(4n+1,2k), 0<=k<=n.
 * https://oeis.org/A193634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(4r+1, 2k) where r = floor((sqrt(8n+1)-1)/2), k = n -
 *            r(r+1)/2. Triangular array T(r,k)=C(4r+1,2k)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ( (((unsigned __int128)(r + 1) * (r + 2)) / 2) <= n )
        r++;

    /* Column index within the row. */
    idx_t k = n - r * (r + 1) / 2;

    /* Return the binomial coefficient for this position. */
    return binom(4 * r + 1, 2 * k);
}
