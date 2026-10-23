/*
 * A127829  Number triangle mod(C(floor(k/2),n-k),2).
 * https://oeis.org/A127829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = C( floor(k/2), r − k ) mod 2 where r is the unique
 *            integer with r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 and k = n −
 *            r*(r+1)/2. This reproduces the triangle entries T(r,k) =
 *            C(⌊k/2⌋, r−k) (mod 2) flattened row by row
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* compute r = floor((sqrt(8*n+1)-1)/2) and adjust to satisfy
       r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    double dn = (double)n;
    double r_est = floor((sqrt(8.0*dn + 1.0) - 1.0) / 2.0);
    idx_t r = (idx_t)r_est;

    /* adjust r in case of rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    while (r * (r + 1) / 2 > n)
        --r;

    /* compute column k */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* binomial parity: C(a,b) mod 2 == 1 iff (b & ~a) == 0 */
    idx_t a_bin = k >> 1;          /* floor(k/2) */
    idx_t b_bin = r - k;           /* r - k */
    seq_t res = ((b_bin & ~a_bin) == 0) ? 1 : 0;
    return res;
}
