/*
 * A075349  a(1) = 1; first differences follow the pattern
 *          1,2,2,3,3,3,4,4,4,4,5,5,5,5,5,..., i.e., the next n differences
 *          are n.
 * https://oeis.org/A075349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=1}^m k² + (n-1 - m(m+1)/2)*(m+1) where m =
 *            floor((√(8(n-1)+1)-1)/2) (i.e. the largest m with m(m+1)/2 ≤
 *            n-1)
 * Range:     u8 n <= 42, u16 n <= 1691, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute A075349(n).  The function works for any idx_t (uint64_t) and any value type seq_t, including
   zero‑, one‑, two‑, …‑bit unsigned integers and arbitrary‑precision integer classes. */
seq_t a(idx_t n)
{
    if (n == 0) /* not part of the official sequence but keeps the function defined for n=0 */
        return 0;

    /* Convert the index differences into a 64‑bit unsigned integer. */
    const idx_t prev = n - 1;                 /* number of differences already used */

    /* Find the largest m such that m(m+1)/2 ≤ prev.  This is done by a small binary search
       using 128‑bit intermediate arithmetic to avoid overflow. */
    unsigned long long lo = 0;
    unsigned long long hi = 1;
    /* Grow hi until it exceeds the required value. */
    while ((unsigned __int128)hi * (hi + 1) / 2 <= prev)
        hi <<= 1;
    /* Binary search between lo and hi. */
    while (lo < hi) {
        unsigned long long mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= prev)
            lo = mid;
        else
            hi = mid - 1;
    }
    const unsigned long long m = lo;          /* block index */

    /* Remaining differences in the current block. */
    const unsigned long long used = (unsigned __int128)m * (m + 1) / 2;
    const unsigned long long r = prev - used;          /* 0 ≤ r < m+1 */

    /* Sum of squares 1² + 2² + … + m² */
    const unsigned __int128 sumSquares =
        (unsigned __int128)m * (m + 1) * (2 * m + 1) / 6;

    /* Total value: 1 (initial term) + sum of completed squares
       + r differences of value m+1. */
    const unsigned __int128 total =
        1 + sumSquares + (unsigned __int128)r * (m + 1);

    return (seq_t)total;
}
