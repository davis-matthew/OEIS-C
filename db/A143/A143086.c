/*
 * A143086  Triangle read by rows: T(n,k) = 2^(k + 1) - 1 if k < = floor(n/2),
 *          otherwise 2^(n - k + 1) - 1, for 0 <= k <= n.
 * https://oeis.org/A143086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   a(n) = T(r,k) where r is the row index, k the column index. *
 *            Row r is the largest integer with r*(r+1)/2 <= n. * k = n -
 *            r*(r+1)/2. * T(r,k) = 2^(k+1)-1 if k <= floor(r/2) * 2^(r-k+1)-1
 *            otherwise
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/pow2.h"

static inline idx_t row_of_idx(idx_t n)
{
    /* Rough upper bound for the row index: r ≈ √(2n). */
    unsigned long long high = (unsigned long long)(sqrtl((long double)2 * n) + 2);
    idx_t low = 0;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = row_of_idx(n);
    idx_t start = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - start;
    if (k <= r / 2)
        return pow2(k + 1) - 1;
    else
        return pow2(r - k + 1) - 1;
}
