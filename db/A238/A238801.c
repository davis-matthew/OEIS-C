/*
 * A238801  Triangle T(n,k), read by rows, given by T(n,k) = C(n+1, k+1)*(1-(k
 *          mod 2)).
 * https://oeis.org/A238801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (k % 2 == 0) ? binomial(r+1, k+1) : 0 where r =
 *            floor((sqrt(8*n+1)-1)/2), k = n - r*(r+1)/2 and binomial(n,k) =
 *            C(n,k)
 * Range:     u8 n <= 39, u16 n <= 178, u32 n <= 610, u64 n <= 2307,
 *            u128 n <= 5049, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* find row r: smallest r with r*(r+1)/2 > n, so r-1 is the row */
    double d = sqrt(8.0 * (double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);

    /* adjust in case of floating‑point rounding errors */
    while (( (__int128)r * (r + 1) / 2 ) > n) r--;
    while (( (__int128)(r + 1) * (r + 2) / 2 ) <= n) r++;

    /* column index within the row */
    idx_t k = n - (__int128)r * (r + 1) / 2;

    /* entries with odd k are zero */
    if (k & 1) return (seq_t)0;

    /* even k: return the binomial coefficient */
    return binomial(r + 1, k + 1);
}
