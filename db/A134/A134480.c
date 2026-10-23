/*
 * A134480  Triangle read by rows: T(n,k) = Sum_{i=k..n} n + i.
 * https://oeis.org/A134480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r=floor((sqrt(1+8*n)-1)/2), k=n-r*(r+1)/2,
 *            and T(r,k)=r*(2*r+1)-(r+k)*(r+k-1)/2
 * Range:     u8 n <= 90, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* estimate the row index r using the quadratic formula */
    long double val = 1.0L + 8.0L * (long double)n;
    idx_t r = (idx_t)((sqrtl(val) - 1.0L) / 2.0L);

    /* adjust r so that start <= n < start_next */
    while ((unsigned __int128)r * (r + 1) / 2 > n)
        r--;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        r++;

    /* column offset within the row */
    idx_t start = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - start;

    /* compute T(r,k) with arbitrary–precision intermediates */
    unsigned __int128 rr = r;
    unsigned __int128 kk = k;
    unsigned __int128 term1 = rr * ((rr * 2) + 1);
    unsigned __int128 rk  = rr + kk;
    unsigned __int128 term2 = rk * (rk - 1) / 2;
    unsigned __int128 result = term1 - term2;

    return (seq_t)result;
}
