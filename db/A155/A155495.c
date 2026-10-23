/*
 * A155495  Triangle read by rows: t(n,m) = binomial(2*n,2*m) * binomial(n,m).
 * https://oeis.org/A155495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(2r,2k)*binomial(r,k) with r =
 *            floor((sqrt(8n+1)-1)/2) and k = n - r*(r+1)/2 (triangle read by
 *            rows)
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 95, u64 n <= 309,
 *            u128 n <= 1056, big any n
 * Helpers:   binomial, isqrt_u128_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include <stdint.h>
#include "../utils/isqrt_u128_uint64.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
    unsigned __int128 xx = (unsigned __int128)8 * n + 1;
    idx_t r = (isqrt_u128_uint64(xx) - 1) >> 1;

    /* Offset where this row starts (number of elements before row r) */
    idx_t start = r * (r + 1) / 2;

    /* Column index within the row */
    idx_t k = n - start;

    /* Compute the triangle entry */
    seq_t term1 = binomial((2 * r), (2 * k));
    seq_t term2 = binomial(r, k);

    return term1 * term2;
}
