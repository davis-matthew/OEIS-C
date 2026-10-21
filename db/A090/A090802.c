/*
 * A090802  Triangle read by rows: a(n,k) = number of k-length walks in the
 *          Hasse diagram of a Boolean algebra of order n.
 * https://oeis.org/A090802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 41, u32 n <= 100, u64 n <= 247,
 *            u128 n <= 625, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A090802: triangle read by rows
 *  a(n) = k!*C(r,k)*2^(r-k)
 *  where r = floor((sqrt(8n+1)-1)/2)  (row index)
 *        k = n - r*(r+1)/2           (column index)
 */
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Column k within the row */
    idx_t k = n - r * (r + 1) / 2;

    /* Compute P(r,k) = r*(r-1)*...*(r-k+1) */
    seq_t res = 1;
    for (idx_t i = 0; i < k; ++i)
        res *= (seq_t)(r - i);

    /* Multiply by 2^(r-k) */
    res *= pow2(r - k);

    return res;
}
