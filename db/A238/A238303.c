/*
 * A238303  Triangle T(n,k), 0<=k<=n, read by rows given by T(n,0) = 1, T(n,k)
 *          = 2 if k>0.
 * https://oeis.org/A238303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 1 if n is a triangular number (n = m*(m+1)/2 for some
 *            m>=0), otherwise 2
 * Range:     u8 n <= 9919, u16 n <= 9919, u32 n <= 9919, u64 n <= 9919,
 *            u128 n <= 9919, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A238303: returns 1 at triangular indices, 2 elsewhere.  */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    unsigned __int128 tri_high = (unsigned __int128)high * (high + 1) / 2;
    if (tri_high == n) {
        return 1;
    }
    return 2;
}
