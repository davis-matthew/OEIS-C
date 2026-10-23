/*
 * A023971  First bit in fractional part of binary expansion of 4th root of n.
 * https://oeis.org/A023971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 0 if k^4 ≤ n ≤ k^4 + 2k^3 + (3k^2+k)/2, 1 otherwise,
 *            where k = ⌊n^(1/4)⌋
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_fourth_root(idx_t n)
{
    idx_t low = 0, high = 65536;            /* high exclusive */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;   /* mid² */
        unsigned __int128 mid4 = mid2 * mid2;                   /* mid⁴ */
        if (mid4 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)  /* guard, offset is 1 */
        return (seq_t)0;

    idx_t k = floor_fourth_root(n);

    idx_t k2 = k * k;         /* k² */
    idx_t k3 = k2 * k;        /* k³ */
    idx_t k4 = k2 * k2;       /* k⁴ */

    idx_t bound1 = k4 + 2 * k3 + (3 * k2 + k) / 2;

    return (seq_t)(n <= bound1 ? 0 : 1);
}
