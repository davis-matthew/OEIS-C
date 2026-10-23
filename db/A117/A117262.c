/*
 * A117262  Triangle T, read by rows, where matrix inverse T^-1 has -3^n in
 *          the secondary diagonal: [T^-1](n+1,n) = -3^n, with all 1's in the
 *          main diagonal and zeros elsewhere.
 * https://oeis.org/A117262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor( (sqrt(8n+1)-1)/2 ), k = n -
 *            r(r+1)/2, and T(r,k) = 3^( r*(r-1)/2 - k*(k-1)/2 )
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 27, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"   /* provides pow3(idx_t exp) */

seq_t a(idx_t n)
{
    /* Find the row r such that n lies in [ r(r+1)/2 , (r+1)(r+2)/2 ) */
    idx_t r = 0;
    unsigned __int128 start = 0;            /* r*(r+1)/2 */
    while (start <= n)
    {
        r++;
        start = (unsigned __int128)r * (r + 1) / 2;
    }
    r--;                                     /* r now satisfies start <= n < next_start */
    start = (unsigned __int128)r * (r + 1) / 2;

    /* Position within the row */
    idx_t k = (idx_t)(n - start);

    /* Compute exponent e = r*(r-1)/2 - k*(k-1)/2 */
    unsigned __int128 e_r   = (unsigned __int128)r * (r - 1) / 2;
    unsigned __int128 e_k   = (unsigned __int128)k * (k - 1) / 2;
    seq_t e = (seq_t)(e_r - e_k);

    return pow3((idx_t)e);
}
