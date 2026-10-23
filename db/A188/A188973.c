/*
 * A188973  Zero-one sequence based on squares: a(A000290(k))=a(k);
 *          a(A000037(k))=1-a(k); a(1)=0.
 * https://oeis.org/A188973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(floor_sqrt(n)) if n is a square; * a(n) = 1 - a(n -
 *            floor_sqrt(n)) otherwise
 * Range:     u8 n <= 135, u16 n <= 135, u32 n <= 135, u64 n <= 135,
 *            u128 n <= 135, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of a 64‑bit unsigned integer
   (returns the largest x such that x*x <= n) */
static inline idx_t isqrt_idx_t(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 32;   /* sqrt(2^64-1) < 2^32 */
    idx_t res = 0;
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n) {
            res = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    unsigned invert = 0;   /* 0 => use normal value, 1 => use complement */
    idx_t m = n;
    while (m != 1)
    {
        idx_t s = isqrt_idx_t(m);
        if (s * s == m)
            m = s;          /* perfect square: descend to sqrt(n) */
        else {
            invert ^= 1;    /* complement for non‑square */
            m = m - s;      /* m := index of current value */
        }
    }
    return (seq_t)(invert ? 1 : 0);   /* a(1) = 0; toggle if needed */
}
