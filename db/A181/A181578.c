/*
 * A181578  The number k such that each of the five factorials (5k+j)!,
 *          j=0..4, has exactly n trailing zeros in its base-10
 *          representation; 0 if no such k exists.
 * https://oeis.org/A181578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   trailing_zeros_of_5k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A181578:  a(n) = the smallest k such that the trailing‑zero count of (5k)! equals n.
    If no such k exists, a(n) = 0.
    The trailing‑zero count of (5k)! is
        f(k) = k + floor(k/5) + floor(k/25) + floor(k/125) + …
    The sequence is computed by finding the inverse of this monotone function. */

#include <stdint.h>
#include "../utils/trailing_zeros_of_5k.h"

seq_t a(idx_t n)
{
    /* Special case: n = 0  →  k = 0 is the unique solution. */
    if (n == 0)
        return (seq_t)0;

    /* Establish bounds for binary search.
       f(k) >= k, so k ≤ n is always sufficient (for n > 0). */
    idx_t low = 0;
    idx_t high = n;                    /* at worst k = n satisfies f(k) > n */

    while (low < high) {
        idx_t mid = (low + high) >> 1;
        seq_t val = trailing_zeros_of_5k(mid);
        if (val < n)
            low = mid + 1;
        else
            high = mid;
    }

    /* after the loop 'low' is the smallest k with f(k) >= n */
    if (trailing_zeros_of_5k(low) == n)
        return (seq_t)low;
    return (seq_t)0;
}
