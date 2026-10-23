/*
 * A121335  Triangle, read by rows, where T(n,k) = C( n*(n+1)/2 + n-k + 1,
 *          n-k), for n>=k>=0.
 * https://oeis.org/A121335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) = binom(r*(r+1)/2 + r - k + 1, r -
 *            k), r >= k >= 0, r is the largest integer with r*(r+1)/2 <= n, k
 *            = n - r*(r+1)/2
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 44, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that offset(r) <= n < offset(r+1) */
    idx_t low = 0, high = 1;

    /* Increase high until its offset exceeds n */
    while (1) {
        unsigned __int128 offset = (unsigned __int128)high * (high + 1) / 2;
        if (offset > (unsigned __int128)n)
            break;
        if (high > UINT64_MAX / 2) {
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }

    /* Binary search for the greatest r with offset(r) <= n */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 offset_mid = (unsigned __int128)mid * (mid + 1) / 2;
        if (offset_mid <= (unsigned __int128)n)
            low = mid + 1;
        else
            high = (mid == 0) ? 0 : mid - 1;
    }
    idx_t r = high;

    /* Compute the position k within the row */
    unsigned __int128 offset_r = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)offset_r;

    /* Binomial parameters: a = offset_r + (r - k) + 1, b = r - k */
    unsigned __int128 a_binom = offset_r + (unsigned __int128)(r - k) + 1;
    idx_t b_binom = r - k;

    return binom((idx_t)a_binom, b_binom);
}
