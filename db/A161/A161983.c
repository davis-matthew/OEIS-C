/*
 * A161983  Irregular triangle read by rows: the group of 2n + 1 integers
 *          starting at A014105(n).
 * https://oeis.org/A161983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = n + floor(sqrt(n))*(floor(sqrt(n)) + 1);
 * Range:     u8 n <= 31, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: integer floor square root of a 64‑bit unsigned integer.
   Implements a binary search; safe for the maximum value of idx_t. */
static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        /* mid <= sqrt(n) so mid*mid fits in 64 bits */
        seq_t prod = (seq_t)mid * (seq_t)mid;
        if (prod <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    idx_t r = floor_sqrt_u64(n);
    /* r*(r+1) fits in 64 bits because r <= sqrt(n) <= 2^32-1 */
    return (seq_t)n + (seq_t)r * ((seq_t)r + 1);
}
