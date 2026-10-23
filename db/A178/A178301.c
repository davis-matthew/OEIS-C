/*
 * A178301  Triangle T(n,k) = binomial(n,k)*binomial(n+k+1,n+1) read by rows,
 *          0 <= k <= n.
 * https://oeis.org/A178301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 40, u32 n <= 127, u64 n <= 421,
 *            u128 n <= 1465, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A178301: Triangle T(n,k) = binomial(n,k)*binomial(n+k+1,n+1) read by rows.
   a(n) = T(r,k) where r satisfies r*(r+1)/2 <= n < (r+1)*(r+2)/2
   and k = n - r*(r+1)/2. */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* binary search for row r */
    idx_t low = 0, high = n + 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;

    /* starting index of row r */
    unsigned __int128 start128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)start128;

    /* compute binomial factors */
    seq_t first  = binom(r, k);                /* binomial(r,k)   */
    seq_t second = binom(r + k + 1, r + 1);    /* binomial(r+k+1,r+1) */

    return first * second;
}
