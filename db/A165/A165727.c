/*
 * A165727  Table T(k,n) read by antidiagonals: denominator of 1/min(n,k)^2
 *          -1/max(n,k)^2 with T(0,n) = T(k,0) = 0.
 * https://oeis.org/A165727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, frac, easy
 * Formula:   a(n) = T(k,n) where (k,n) are obtained by reading * Table T(k,n)
 *            by antidiagonals (k+n constant). * T(k,n) is defined as: * 0 if
 *            k==0 or n==0, * 1 if k==n (>0), * denom of 1/min(k,n)^2 -
 *            1/max(k,n)^2 otherwise
 * Range:     u8 n <= 47, u16 n <= 573, u32 n <= 1034, u64 n <= 1034,
 *            u128 n <= 1034, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Integer square root for 64‑bit values, floor(sqrt(x)). */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = (1ULL << 32);
    if (hi > x) hi = x;                /* hi <= x unless x < 2^32 */
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;                          /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* Find the diagonal d such that
     * d*(d+1)/2 <= n < (d+1)*(d+2)/2. */
    idx_t d = ((isqrt_u64(8*n + 1) - 1) / 2);

    /* Triangular number of previous diagonal. */
    idx_t prev = d * (d + 1) / 2;
    idx_t offset = n - prev;

    /* Coordinates on the diagonal. */
    idx_t k = offset;             /* row index */
    idx_t m = d - offset;         /* column index */

    if (k == 0 || m == 0)
        return 0;

    if (k == m)
        return 1;

    /* Compute denominator of 1/min^2 - 1/max^2. */
    seq_t a1 = (seq_t)k;
    seq_t a2 = (seq_t)m;
    seq_t lo = a1 < a2 ? a1 : a2;
    seq_t hi = a1 < a2 ? a2 : a1;

    seq_t num = (hi - lo) * (hi + lo);        /* M^2 - m^2 */
    seq_t den = lo * lo;
    den *= hi * hi;

    seq_t g = gcd(num, den);
    return den / g;
}
