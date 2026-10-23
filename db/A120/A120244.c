/*
 * A120244  a(1) = 1. a(m(m+1)/2 + k) = a(m) + k, 1 <= k <= m+1, m >= 1.
 * https://oeis.org/A120244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9953, u16 n <= 9953, u32 n <= 9953, u64 n <= 9953,
 *            u128 n <= 9953, big any n
 * Helpers:   _tri_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1.
   For m≥1 and 1≤k≤m+1,
   a(m(m+1)/2 + k) = a(m) + k.
   Equivalently, let T(m)=m(m+1)/2.  For n>1 let
   m = max{t : T(t) < n}, k = n - T(m).  Then a(n)=a(m)+k.
   The following function evaluates a(n) in O(log n) time. */

#include <stdint.h>
#include "../utils/_tri_idx.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;          /* a(1)=1, a(0) undefined but unused */

    idx_t cur = n;
    seq_t offset = 0;

    while (cur > 1)
    {
        /* Binary search for greatest m with T(m) < cur */
        idx_t low = 0;
        idx_t high = cur;                /* safe upper bound */
        while (low < high)
        {
            idx_t mid = (low + high + 1) >> 1;  /* upper mid to avoid endless loop */
            if (_tri_idx(mid) < cur)
                low = mid;
            else
                high = mid - 1;
        }
        idx_t m = low;                      /* T(m) < cur ≤ T(m+1) */
        idx_t k = cur - (idx_t)_tri_idx(m); /* 1 ≤ k ≤ m+1 */
        offset += (seq_t)k;
        cur = m;                            /* recurse on m */
    }

    return (seq_t)1 + offset;  /* a(1)=1 + accumulated offsets */
}
