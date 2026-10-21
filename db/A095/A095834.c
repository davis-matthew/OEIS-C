/*
 * A095834  Triangle read by rows: T(n,k) = (n-k)*n, n>=1, 1<=k<=n.
 * https://oeis.org/A095834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (row - k) * row where row is chosen so that n lies in the
 *            flattened triangle and k = n - (row-1)*row/2
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* just in case, offset is 1 */

    /* Find row r such that r*(r+1)/2 >= n > (r-1)*r/2.
       Binary search over 1..n (no overflow because we use __int128). */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val < n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low;

    /* Compute the index within the row (1-based) */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev;   /* 1 <= k <= r */

    /* Convert to seq_t for final value */
    seq_t rn = (seq_t)r;
    seq_t kn = (seq_t)k;
    return rn * (rn - kn);
}
