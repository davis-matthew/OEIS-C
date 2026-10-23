/*
 * A157636  Triangle read by rows: T(n, k) = 1 if k=0 or k=n, otherwise =
 *          n*k*(n-k)/2.
 * https://oeis.org/A157636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 if k==0 or k==n a(n) = n * k * (n-k) / 2 otherwise
 *            where n is the row index found from the triangular index and k =
 *            n - r*(r+1)/2 is the column offset within that row
 * Range:     u8 n <= 95, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* binary search for the largest r such that r*(r+1)/2 <= n */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1) / 2;
        if (prod <= n) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    /* offset within the row */
    unsigned __int128 start = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)start;

    /* compute the value */
    if (k == 0 || k == r)
        return (seq_t)1;

    seq_t rs = (seq_t)r;
    seq_t ks = (seq_t)k;
    return rs * ks * (rs - ks) / 2;
}
