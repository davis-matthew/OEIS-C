/*
 * A073189  Integers 0..k three times, then 0..k+1 three times, etc.
 * https://oeis.org/A073189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is obtained by finding the block index k such that * C(k-1)
 *            ≤ n < C(k), where C(k) = 3*(k+1)*(k+2)/2 is the total * number
 *            of terms up to and including block k. Then * a(n) = (n - C(k-1))
 *            mod (k+1)
 * Range:     u8 n <= 3977, u16 n <= 3977, u32 n <= 3977, u64 n <= 3977,
 *            u128 n <= 3977, big any n
 * Helpers:   C
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/C.h"

seq_t a(idx_t n)
{
    /* Find the minimal k with C(k) > n. */
    idx_t low = 0, high = 1;
    while (C(high) <= n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (C(mid) <= n) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    idx_t k = low;                      /* block that contains index n */
    idx_t cum_before = (k == 0) ? 0 : C(k - 1);
    idx_t d = n - cum_before;           /* offset within the block k */
    idx_t period = k + 1;               /* length of the repeated pattern */
    return (seq_t)(d % period);
}
