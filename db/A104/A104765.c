/*
 * A104765  Triangle T(n,k) read by rows: row n contains the first n Lucas
 *          numbers A000204.
 * https://oeis.org/A104765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = Lucas( n - t(t+1)/2 ) where t = floor((sqrt(8n-7)-1)/2)
 * Range:     u8 n <= 77, u16 n <= 299, u32 n <= 1127, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   lucas_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/lucas_at.h"

seq_t a(idx_t n)
{
    /* Find t such that t(t+1)/2 < n <= (t+1)(t+2)/2 */
    idx_t low  = 0;
    idx_t high = 1;
    /* enlarge high until its triangular number is >= n */
    while ((unsigned __int128)high * (high + 1) / 2 < n) {
        high <<= 1;
    }
    /* binary search for the largest t with triangular number < n */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri < n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t t   = low;
    unsigned __int128 tri = (unsigned __int128)t * (t + 1) / 2;
    idx_t m = n - (idx_t)tri;          /* position in the row */
    return lucas_at(m);
}
