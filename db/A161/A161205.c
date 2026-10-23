/*
 * A161205  Triangle read by rows in which row n lists 2n-1 followed by 2n
 *          numbers 2n.
 * https://oeis.org/A161205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = floor(sqrt(n-1)) + floor(sqrt(n))
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer floor square root for 64‑bit unsigned indices.
   Uses binary search with 128‑bit multiplication to avoid overflow. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = (idx_t)1 << 32;       /* (2^32)^2 = 2^64 > maximum n */
    if (high > n) high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;                       /* floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    if (n == 0)            /* logic above offset 1 - returned value ignored */
        return 0;
    idx_t s1 = isqrt_u64(n - 1);
    idx_t s2 = isqrt_u64(n);
    return (seq_t)s1 + (seq_t)s2;
}
