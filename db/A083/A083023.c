/*
 * A083023  a(n) = number of partitions of n into a pair of parts n=p+q,
 *          p>=q>=0, with p-q equal to a square >= 0.
 * https://oeis.org/A083023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = # { k | 0 ≤ k ≤ n/2 , n-2k is a square } Equivalently
 *            a(n) = # { s | 0 ≤ s ≤ √n , s parity = n parity }
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Integer square root for idx_t (returns floor(sqrt(n))).
   Uses binary search with division to avoid overflow. */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid == 0) {
            if (n == 0) return 0;
            low = 1;
            continue;
        }
        if (mid > n / mid) {
            high = mid - 1;
        } else {
            ans = mid;
            low = mid + 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset starts at 1, but guard anyway */
    idx_t m = isqrt_idx(n);           /* floor(sqrt(n)) */

    if ((n & 1ULL) == 0) {            /* n even: count even s (including 0) */
        return (seq_t)(m / 2 + 1);
    } else {                          /* n odd: count odd s */
        return (seq_t)((m + 1) / 2);
    }
}
