/*
 * A132710  Infinitesimal generator for a diagonally-shifted Lah matrix,
 *          unsigned A105278, related to n! Laguerre(n,-x,1).
 * https://oeis.org/A132710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = k(k+1) where k = (sqrt(8*n+17)-3)/2 if sqrt is an
 *            integer, otherwise a(n) = 0. (a(0)=0)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned 128‑bit values.
   Returns the greatest integer r such that r*r <= x. */
static uint64_t isqrt_u128(unsigned __int128 x)
{
    uint64_t lo = 0, hi = (uint64_t)-1;      /* 2^64-1 is safe upper bound */

    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;

        if (mid2 == x)
            return mid;
        if (mid2 < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;   /* hi is floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    if (n == 0)            /* offset 0: a(0) = 0 */
        return 0;

    /* Compute m = 8*n + 17 in 128‑bit to avoid overflow for large n. */
    unsigned __int128 m = (unsigned __int128)8 * n + 17;

    /* Integer square root of m */
    uint64_t s = isqrt_u128(m);
    if ((unsigned __int128)s * s != m)   /* not a perfect square */
        return 0;

    /* Check that (s-3) is even and non‑negative */
    if (s < 3 || ((s - 3) & 1))
        return 0;

    uint64_t k = (s - 3) >> 1;          /* k = (s-3)/2 */
    if (k == 0)                         /* should never happen for n>0 */
        return 0;

    /* Compute k(k+1) in seq_t type. */
    seq_t kseq = (seq_t)k;
    return kseq * (kseq + 1);
}
