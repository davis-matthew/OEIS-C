/*
 * A094765  a(n) = n + 2 * square excess of n.
 * https://oeis.org/A094765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n - 2*⌊√n⌋² (OEIS A094765: a(n) = n + 2*A053186(n))
 * Range:     u8 n <= 215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned 64‑bit integers (floor).
   Works for any n >= 0. Uses binary search with 128‑bit arithmetic to avoid overflow. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;                 /* hi is floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    /* Compute r = floor(sqrt(n)) */
    idx_t r = isqrt_u64(n);

    /* Compute r² without overflow in 64‑bit builds. In arbitrary‑precision
       builds r² also fits in seq_t. */
    seq_t rsq = (seq_t)r * r;

    /* a(n) = 3*n - 2*r² */
    return (seq_t)n * (seq_t)3 - rsq * (seq_t)2;
}
