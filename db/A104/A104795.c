/*
 * A104795  Triangle T(n,k) = C(n,k)+1 for k<n; T(n,k) = 1 for k=n, read by
 *          rows.
 * https://oeis.org/A104795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binom(r,k)+1 when k<r, a(n) = 1 when k==r, where r =
 *            floor((sqrt(8*n+1)-1)/2), k = n - r(r+1)/2
 * Range:     u8 n <= 39, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    if (k == r)
        return (seq_t)1;           /* last element of the row */

    seq_t val = binom(r, k);      /* C(r, k) */
    return val + (seq_t)1;        /* add 1 as per definition */
}
