/*
 * A098473  Triangle T(n,k) read by rows, T(n, k) = binomial(2*k,
 *          k)*binomial(n, k), 0<=k<=n.
 * https://oeis.org/A098473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(2*k, k) * binomial(m, k), where (m, k) is the
 *            unique pair of integers with m*(m+1)/2 ≤ n < (m+1)*(m+2)/2 and k
 *            = n - m*(m+1)/2
 * Range:     u8 n <= 13, u16 n <= 43, u32 n <= 147, u64 n <= 488,
 *            u128 n <= 1756, big any n
 * Helpers:   binomial, tri_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/tri_val.h"

seq_t a(idx_t n)
{
    /* Find the row index m such that tri_val(m) > n.
       Then row number is m - 1. */
    idx_t m = 0;
    while (tri_val(m) <= n)
        ++m;
    idx_t row = m - 1;
    idx_t prev_tri = (unsigned __int128)(row) * (row + 1) / 2;
    idx_t k = n - prev_tri;                     /* 0 ≤ k ≤ row */

    idx_t two_k = k + k;                       /* 2*k, fits in idx_t for test ranges */

    seq_t part1 = binomial(two_k, k);          /* binomial(2*k, k) */
    seq_t part2 = binomial(row, k);            /* binomial(m, k)  */
    return part1 * part2;
}
