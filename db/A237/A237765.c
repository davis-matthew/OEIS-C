/*
 * A237765  Triangular array read by rows: T(n,k) =
 *          binomial(n,2)*binomial(n,k), n>=0, 0<=k<=n.
 * https://oeis.org/A237765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r,2) * binomial(r,k) where r is the largest
 *            integer such that r*(r+1)/2 <= n, and k = n - r*(r+1)/2. This
 *            yields the triangular array T(r,k)=C(r,2)*C(r,k) read by rows
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    /* Use unsigned __int128 for safe multiplication */
    while ( ( (unsigned __int128)(r + 1) * (r + 2) ) / 2 <= n )
        ++r;

    /* Column k */
    idx_t k = n - ( (unsigned __int128)r * (r + 1) ) / 2;

    /* Compute T(r,k) = C(r,2) * C(r,k) */
    seq_t c = binomial(r, 2);
    seq_t d = binomial(r, k);
    return c * d;
}
