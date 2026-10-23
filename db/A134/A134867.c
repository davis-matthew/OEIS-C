/*
 * A134867  A010766 * A000012.
 * https://oeis.org/A134867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{t=k}^{r} floor(r/t), where r = minimal integer with
 *            r(r+1)/2 >= n and k = n - r(r-1)/2
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety for non‑positive n */

    /*--- Find the row index r ---*/
    idx_t low  = 1;
    idx_t high = 1;
    /* grow high until triangular(high) >= n */
    while ((__int128)high * (high + 1) / 2 < n)
        high <<= 1;

    /* binary search between low and high */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /*--- Determine k = column index within the row ---*/
    __int128 prev_tri128 = (__int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev_tri128;

    /*--- Sum floor(r/t) for t = k .. r ---*/
    seq_t sum = 0;
    for (idx_t t = k; t <= r; ++t)
        sum += (seq_t)r / (seq_t)t;

    return sum;
}
