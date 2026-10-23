/*
 * A131239  Triangle, T(n,k) = 3*A007318(n,k) - 2*A046854(n,k), read by rows.
 * https://oeis.org/A131239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = T(r,k) where * r = floor((sqrt(8*n+1)-1)/2) (row index)
 *            * k = n - r*(r+1)/2 (column index) * T(r,k) = 3*binomial(r,k) -
 *            2*binomial(floor((r+k)/2), k) * binomial(x,y) is C(x,y) with
 *            standard elementary definition
 * Range:     u8 n <= 48, u16 n <= 160, u32 n <= 609, u64 n <= 2241,
 *            u128 n <= 5150, big any n
 * Helpers:   binomial_safe
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial_safe.h"

seq_t a(idx_t n)
{
    /* Locate the row r that contains position n.  The total number of
       elements up to and including row r is r*(r+1)/2.  We look for the
       largest r such that this triangular number is <= n. */
    idx_t r = 0;
    while (((seq_t)(r + 1) * (r + 2)) / 2 <= (seq_t)n) {
        ++r;
    }

    /* Column index k within that row */
    idx_t k = n - (idx_t)(((seq_t)r * (r + 1)) / 2);

    /* Compute the triangle entry using the given formula. */
    seq_t bin1 = binomial_safe(r, k);            /* binomial(r, k)   */
    seq_t res   = bin1 * (seq_t)3;               /* 3*bin(r,k)       */

    idx_t mid   = (r + k) / 2;                    /* floor((r+k)/2)   */
    seq_t bin2  = binomial_safe(mid, k);          /* bin(mid,k)       */

    res -= bin2 * (seq_t)2;                      /* subtract 2*bin   */
    return res;
}
