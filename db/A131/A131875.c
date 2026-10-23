/*
 * A131875  Triangle, A000012 * A131844 as infinite lower triangular matrices.
 * https://oeis.org/A131875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = pentagonal(k+1) if j==0 else k+1+2*j, where k =
 *            floor((sqrt(8*n+1)-1)/2) and j = n - k*(k+1)/2
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row k such that k*(k+1)/2 <= n < (k+1)*(k+2)/2 */
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n) high <<= 1;

    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t k = low;

    /* Position j within the row */
    idx_t j = n - (unsigned __int128)k * (k + 1) / 2;

    /* Compute the entry */
    if (j == 0) {
        unsigned __int128 val = (unsigned __int128)(k + 1) *
                                ((unsigned __int128)3 * k + 2) / 2;
        return (seq_t)val;
    } else {
        unsigned __int128 val = (unsigned __int128)(k + 1) +
                                (unsigned __int128)2 * j;
        return (seq_t)val;
    }
}
