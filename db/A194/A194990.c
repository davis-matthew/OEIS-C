/*
 * A194990  a(n) = 1+ floor(n/sqrt(8)).
 * https://oeis.org/A194990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor(n / sqrt(8))
 * Range:     u8 n <= 721, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned __int128.
 * Returns the largest r with r*r <= m. */
static unsigned __int128 isqrt128(unsigned __int128 m)
{
    unsigned __int128 lo = 0;
    unsigned __int128 hi = ((unsigned __int128)1 << 64);   /* 2^64 */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        unsigned __int128 mid2 = mid * mid;
        if (mid2 <= m)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1 (offset 1) */
    if (n == 0) return 0;                    /* safety value, not used */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 m = nn * nn / 8;        /* floor(n^2 / 8)   */
    unsigned __int128 r = isqrt128(m);        /* floor(sqrt(m))   */
    return (seq_t)(r + 1);                    /* a(n) = 1 + r    */
}
