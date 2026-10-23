/*
 * A190650  Product of iterated integral part of square root.
 * https://oeis.org/A190650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * a(floor(sqrt(n))) for n > 1, with a(1) = 1
 * Range:     u8 n <= 25, u16 n <= 409, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the iterative chain of square roots */
/* Complexity: O(log n) per call                                                */
#include <stdint.h>

/* Integer square root – floor(sqrt(n)) for 64‑bit input                     */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* Compute a(n) as defined by OEIS A190650                            */
seq_t a(idx_t n)
{
    seq_t result = (seq_t)n;
    idx_t cur = isqrt_u64(n);
    while (cur > 1) {
        result = result * (seq_t)cur;
        cur = isqrt_u64(cur);
    }
    return result;
}
