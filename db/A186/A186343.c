/*
 * A186343  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f and g are the pentagonal numbers and
 *          the octagonal numbers. Complement of A186342.
 * https://oeis.org/A186343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((1 + sqrt(1 + 24 * (n*(3n-2))))/6) + floor((2 +
 *            sqrt(4 + 12 * (n*(3n-2))))/6)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit value */
static inline unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 l = 0, r = (unsigned __int128)1 << 64;
    while (l < r) {
        unsigned __int128 m = (l + r + 1) >> 1;
        if (m * m <= x) l = m; else r = m - 1;
    }
    return l;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    unsigned __int128 n128 = (unsigned __int128)n;
    unsigned __int128 X  = n128 * ((unsigned __int128)3 * n128 - 2);       /* g(n) = n*(3n‑2) */
    unsigned __int128 Dp = 1 + (unsigned __int128)24 * X;                  /* 1 + 24*X */
    unsigned __int128 sqrtDp = isqrt128(Dp);
    unsigned __int128 cntP = (1 + sqrtDp) / 6;                             /* # pentagonal ≤ X */

    unsigned __int128 Do  = 4 + (unsigned __int128)12 * X;                  /* 4 + 12*X */
    unsigned __int128 sqrtDo = isqrt128(Do);
    unsigned __int128 cntO = (2 + sqrtDo) / 6;                             /* # octagonal ≤ X */

    unsigned __int128 ans = cntP + cntO;
    return (seq_t)ans;
}
