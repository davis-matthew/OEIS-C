/*
 * A238013  List n copies of each k in {1,2,...,n}, for n=1,2,3,...
 * https://oeis.org/A238013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A237452(n) + 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_sq128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/sum_sq128.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* unused, but harmless */

    /* Find the block index k such that sum_{i=1}^{k-1} i^2 < n ≤ sum_{i=1}^{k} i^2 */
    idx_t low = 1;
    idx_t high = 1;
    unsigned __int128 target = n;

    /* Increase high until it covers the target index */
    while (sum_sq128(high) < target) {
        if (high > ((idx_t)-1) >> 1) break; /* avoid overflow */
        high <<= 1;
    }

    /* Binary search for the smallest k with sum_{i=1}^{k} i^2 ≥ n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (sum_sq128(mid) >= target)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t k = low;                     /* block index */

    /* Compute offset of n within block k (0‑based) */
    unsigned __int128 sumPrev128 = sum_sq128(k - 1);
    idx_t sumPrev = (idx_t)sumPrev128;
    idx_t offset   = n - sumPrev - 1;

    /* Value in block k is offset/k + 1 */
    return (seq_t)(offset / k + 1);
}
