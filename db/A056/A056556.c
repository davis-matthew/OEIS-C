/*
 * A056556  First tetrahedral coordinate; repeat m (m+1)*(m+2)/2 times.
 * https://oeis.org/A056556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = m if n >= binomial(m+2,3) else m-1, where m = floor((6*n
 *            + 6)^(1/3)). This yields the first tetrahedral coordinate as
 *            described in OEIS A056556. The implementation uses integer
 *            arithmetic only, with a binary-search cube root and 128-bit
 *            intermediates to avoid overflow
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute (6*n + 6) as a 128‑bit value to avoid overflow. */
    unsigned __int128 val = (unsigned __int128)6 * n + 6;

    /* Integer cube root floor of val using binary search. */
    unsigned __int128 low  = 0;
    unsigned __int128 high = 1;
    /* Increase high until high^3 > val */
    while (high * high * high <= val)
        high <<= 1;

    while (low + 1 < high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 cube = mid * mid * mid;
        if (cube <= val)
            low = mid;
        else
            high = mid;
    }
    /* low is floor(cuberoot(val)).  It fits in 64 bits for all
       n considered in typical tests. */
    unsigned long long m = (unsigned long long)low;

    /* Compute binomial(m+2,3) = (m+2)*(m+1)*m / 6 as 128‑bit. */
    unsigned __int128 c128 = (unsigned __int128)(m + 2) * (m + 1) * m / 6;

    /* Determine the correct value: m if n >= binom(m+2,3) else m-1. */
    if ((unsigned __int128)n >= c128)
        return (seq_t)m;
    else
        return (seq_t)(m - 1);
}
