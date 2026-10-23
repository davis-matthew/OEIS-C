/*
 * A129185  Shift operator, left.
 * https://oeis.org/A129185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy, less
 * Formula:   a(n) = 1 if n = k*(k+3)/2 for some integer k ≥ 1, otherwise 0
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 64‑bit unsigned integers. */
static inline idx_t isqrt_u64(idx_t x)
{
    if (x < 2) return x;
    idx_t low = 1, high = x >> 1, mid, div;
    while (low <= high) {
        mid = low + (high - low) / 2;
        div = x / mid;
        if (mid == div) {
            return mid;
        } else if (mid < div) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* a(1) = 0 */
    /* Compute t = 9 + 8*n */
    idx_t t = 9 + 8 * n;
    idx_t s = isqrt_u64(t);
    if (s * s != t) return (seq_t)0;     /* t is not a perfect square */
    if (((s - 3) & 1ULL) == 0) return (seq_t)1; /* (s-3) even → solution exists */
    return (seq_t)0;
}
