/*
 * A137225  Triangle T(k,q) of minimal q-Niven numbers: smallest number such
 *          that the sum of its digits in base q equals k, 2<=q<=k+1.
 * https://oeis.org/A137225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   min_niven_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) = T(k,q) where
 *   k = smallest integer with k*(k+1)/2 ≥ n
 *   offset = n - k*(k-1)/2
 *   q      = offset + 1
 *   T(k,q) = minimal positive integer whose sum of digits in base q = k
 *
 *   T(k,q) is computed by greedily filling the low-order digits with
 *   as much as possible (≤ q-1) to minimize the numeric value.
 */

#include <stdint.h>
#include <stdbool.h>   /* for bool, true, false */
#include "../utils/min_niven_value.h"

seq_t
a(idx_t n)
{
    /* find k such that k*(k+1)/2 >= n */
    idx_t lo = 1, hi = n;   /* n ≥ 1 */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1ULL) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t k = lo;

    /* compute offset in row k and corresponding base q */
    unsigned __int128 tri_prev = (unsigned __int128)(k - 1) * k / 2;
    idx_t offset = n - (idx_t)tri_prev;   /* 1 <= offset <= k */
    idx_t q = offset + 1;                 /* q ranges from 2 to k+1 */

    return min_niven_value(k, q);
}
