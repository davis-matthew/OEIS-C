/*
 * A033684  1 iff n is a square not divisible by 3.
 * https://oeis.org/A033684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 1 iff n is a perfect square and n % 3 != 0, else 0
 * Range:     u8 n <= 9904, u16 n <= 9904, u32 n <= 9904, u64 n <= 9904,
 *            u128 n <= 9904, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(n)) for a 64‑bit unsigned integer n.
 * Uses binary search with division to avoid overflow. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = (n >> 1) + 1;   /* For n = 0, high = 1 is safe */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1) + 1; /* ceil midpoint */
        if (mid <= n / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = isqrt_u64(n);           /* floor sqrt of n */
    if (r * r != n)                   /* n is not a perfect square */
        return 0;
    if (n % 3 == 0)                   /* square divisible by 3 */
        return 0;
    return 1;
}
