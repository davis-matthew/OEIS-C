/*
 * A090044  Triangle read by rows: T(n,k) = A083093 with 1's and 2's
 *          interchanged.
 * https://oeis.org/A090044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = -C(r,k) mod 3 where (r,k) satisfies n = r*(r+1)/2 + k and
 *            0 <= k <= r
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   binom_mod3_lucas
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_mod3_lucas.h"

/* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
static idx_t
find_row(idx_t n)
{
    idx_t low = 0, high = 1;
    /* Double high until the condition fails. */
    while (((unsigned __int128)high * (high + 1)) / 2 <= n)
        high <<= 1;

    low = high >> 1;          /* r is between low and high-1. */
    idx_t ans = low;

    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (val <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t
a(idx_t n)
{
    idx_t r = find_row(n);
    idx_t k = n - (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
    int bmod3 = binom_mod3_lucas(r, k);
    int val = (bmod3 == 0) ? 0 : 3 - bmod3;   /* negative modulo 3 */
    return (seq_t)val;
}
