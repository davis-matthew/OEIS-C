/*
 * A161385  (1,2)-sequence where n-th run has length n^2.
 * https://oeis.org/A161385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A161382(n) + 1 That is, a(n) alternates between 1 and 2
 *            with the n-th run having length n^2
 * Range:     u8 n <= 13, u16 n <= 3333, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   squaresum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/squaresum.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  // undefined for offset 0

    idx_t lo = 1, hi = n;                  // hi guarantees squaresum(hi) >= n
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        if (squaresum(mid) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    // lo (or hi) is the minimal r with sum of squares >= n
    return ((lo & 1) ? 1 : 2);
}
