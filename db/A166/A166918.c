/*
 * A166918  Triangle T(n,k) read by rows: T(n,0) = n mod 2. T(n,k) = 2^(k-1),
 *          0<k<=n.
 * https://oeis.org/A166918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: A166918 – triangle T read by rows.
   For a given index n find the row r satisfying
   r(r+1)/2 ≤ n < (r+1)(r+2)/2.
   Let c = n – r(r+1)/2.  Then
   * if c == 0  →  a(n) = r mod 2
   * else       →  a(n) = 2^(c−1)   (use pow2 from utils)
*/
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* binary search for largest r with r*(r+1)/2 <= n */
    idx_t low = 0;
    idx_t high = n;          /* inclusive high bound, safe for uint64_t */
    while (low < high)
    {
        idx_t mid = low + (high - low + 1) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t start = (idx_t)((__int128)r * (r + 1) / 2);
    idx_t c = n - start;

    if (c == 0)
        return (seq_t)(r & 1);
    else
        return pow2(c - 1);
}
