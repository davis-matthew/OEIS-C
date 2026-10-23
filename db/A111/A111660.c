/*
 * A111660  a(n) = greatest integer p for which n^p + (n+1)^p > (n+2)^p.
 * https://oeis.org/A111660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max p such that n^p + (n+1)^p > (n+2)^p
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   holds
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include "../utils/holds.h"

seq_t a(idx_t n)
{
    /* Handle trivial cases explicitly. */
    if (n <= 1) return (seq_t)0;

    idx_t low = 0, high = n;      /* a(n) < n for all n >= 2 */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (holds(n, mid))
            low = mid;
        else
            high = mid - 1;
    }
    return (seq_t)low;
}
