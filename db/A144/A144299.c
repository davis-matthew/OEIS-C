/*
 * A144299  Triangle of Bessel numbers read by rows. Row n gives T(n,n),
 *          T(n,n-1), T(n,n-2), ..., T(n,0) for n >= 0.
 * https://oeis.org/A144299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 94, u32 n <= 216, u64 n <= 573,
 *            u128 n <= 1618, big any n
 * Helpers:   T_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A144299 – Triangle of Bessel numbers read by rows.
 *  a(n) = T(r,k) where
 *    r = floor( ( sqrt(8*n + 1) - 1 ) / 2 ),
 *    k = n - r*(r+1)/2,
 *    T(r,k) = 0                  if k > floor(r/2 ),
 *    T(r,k) = r! / ((r-2k)! * k! * 2^k)   otherwise.
 *  (k runs from 0 to r in each row of length r+1; entries beyond
 *   floor(r/2) are zero.) */

#include <stdint.h>
#include <math.h>
#include "../utils/T_n_k.h"

seq_t
a(idx_t n)
{
    /* Find the row 'r' such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    long double dn = (long double)n;
    idx_t r = (idx_t)((sqrtl(8.0L * dn + 1.0L) - 1.0L) / 2.0L);

    /* Adjust for possible rounding errors. */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    idx_t before = r * (r + 1) / 2;
    idx_t k = n - before;

    return T_n_k(r, k);
}
