/*
 * A167752  Hankel transform of A167750.
 * https://oeis.org/A167752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 49, i16 n <= 49, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) is zero for all n that are not a quarter‑square,
   i.e. that are not of the form floor((k+1)^2/4) for some integer k>=0.
   For a quarter‑square n, let k satisfy n = floor((k+1)^2/4).
   The sign of a(n) is +1 except when k ≡ 3 or 5 (mod 8), in which case it is -1.
   This reproduces the Hankel transform of A167750 and gives
   abs(a(n)) = A237347(n) – 2 (1 for quarter squares, 0 otherwise).
*/

#include <stdint.h>

/* Integer square root (floor) for unsigned 64‑bit integers. */
static idx_t u64_isqrt(idx_t x)
{
    idx_t low = 0, high = (idx_t)1 << 32;   /* sqrt(2^64-1) < 2^32 */
    if (x >= (unsigned __int128)high * high) high = x; /* reduce the range for small x */

    while (low + 1 < high)
    {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine if n is a quarter‑square. */
    idx_t four_n = n << 2;            /* 4*n */
    idx_t val     = four_n + 3;       /* 4*n + 3   (max m^2 for a quarter‑square) */
    idx_t m       = u64_isqrt(val);   /* floor(sqrt(4*n+3)) */

    /* m^2 must be at least 4*n to be a quarter‑square. */
    if ((unsigned __int128)m * m < four_n)
        return (seq_t)0;              /* not a quarter‑square */

    /* n = floor((k+1)^2/4)  with k = m-1. */
    idx_t k = m - 1;
    idx_t r = k & 7;                  /* k mod 8 */

    seq_t sign = 1;
    if (r == 3 || r == 5)
        sign = -1;

    return sign;
}
