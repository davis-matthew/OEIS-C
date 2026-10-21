/*
 * A058065  Complement of A057843.
 * https://oeis.org/A058065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(phi*n+sqrt(5))-2 -> a(n) = floor(phi*(n+2)) - 3
 *            Where floor(phi*k) = (k + floor(sqrt(5*k*k))) / 2
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer.
   Returns the largest r such that r*r <= x. */
static unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 lo = 0;
    unsigned __int128 hi = ((unsigned __int128)1 << 64);  /* sqrt(2^128-1) < 2^64 */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* use offset: k = n+2 (n is unsigned 64‑bit) */
    idx_t k = n + (idx_t)2;
    /* compute floor(sqrt(5*k*k)) safely in 128‑bit arithmetic */
    unsigned __int128 sq = (unsigned __int128)5 *
                          (unsigned __int128)k *
                          (unsigned __int128)k;
    unsigned __int128 s = isqrt128(sq);
    /* floor(phi*k) = (k + s) / 2 */
    seq_t fp = ((seq_t)k + (seq_t)s) / 2;
    /* a(n) = floor(phi*(n+2)) - 3 */
    return fp - (seq_t)3;
}
