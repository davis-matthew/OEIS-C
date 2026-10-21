/*
 * A065420  Triangle T(n,k) = binomial(n+2,k+1)*(binomial(n+2,k+1)-1), n >=0,
 *          0 <= k <= n.
 * https://oeis.org/A065420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = binomial(r+2 , k+1) * (binomial(r+2 , k+1) - 1) * where r
 *            = floor((sqrt(8*n+1)-1)/2) is the row index of the triangle *
 *            and k = n - r*(r+1)/2 is the column index within that row. * The
 *            implementation uses an integer search for r avoiding
 *            floating‑point * sqrt so it works with both fixed‑wi...
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 159, u64 n <= 576,
 *            u128 n <= 2240, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2
       Use a simple loop – r grows only like sqrt(n), which is very small
       (for n <= 10000, r <= 141). */
    idx_t r = 0;
    while ( (__int128)(r + 1) * (r + 2) / 2 <= n )
        ++r;

    /* Column index within the row. */
    idx_t tri = (__int128)r * (r + 1) / 2;
    idx_t k   = n - tri;

    /* Compute the binomial coefficient and the triangle value. */
    seq_t bin = binomial(r + 2, k + 1);
    return bin * (bin - 1);
}
