/*
 * A143310  Triangle read by rows, A000012 * A127446, 1 <= k <= n.
 * https://oeis.org/A143310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = k * ⌊r/k⌋ * (⌊r/k⌋ + 1) / 2 where r = ceil( (-1 +
 *            sqrt(1+8*n)) / 2 ), k = n - r*(r-1)/2 (1 ≤ k ≤ r)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 128‑bit numbers, return floor(sqrt(x)) as idx_t */
static idx_t isqrt128(unsigned __int128 x)
{
    unsigned __int128 lo = 0;
    unsigned __int128 hi = (unsigned __int128)1 << 64;   /* > sqrt(max 128‑bit) */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return (idx_t)lo;
}

seq_t a(idx_t n)
{
    /* --- find the row r ----------------------------------------------------- */
    unsigned __int128 sum = (unsigned __int128)n * 8 + 1;    /* 1 + 8*n   */
    idx_t s = isqrt128(sum);                               /* floor sqrt */
    idx_t r = (s - 1) / 2;                                  /* ceil of the root */

    /* correct possible underestimation */
    unsigned __int128 tri = (unsigned __int128)r * (r + 1) / 2;
    if (tri < n) {
        r++;
        tri = (unsigned __int128)r * (r + 1) / 2;
    }

    /* --- column k ----------------------------------------------------------- */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;  /* entries before row r */
    idx_t k = n - (idx_t)prev;                                 /* 1 ≤ k ≤ r */

    /* --- value -------------------------------------------------------------- */
    idx_t m = r / k;                                            /* floor(r/k)   */
    seq_t val = (seq_t)k * ((seq_t)m * (m + 1) / 2);            /* T(r,k)       */

    return val;
}
