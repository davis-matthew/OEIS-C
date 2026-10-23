/*
 * A143999  Rectangular array by antidiagonals: label each unit square in the
 *          first quadrant lattice by its northeast vertex (x,y) and mark
 *          squares for which (x,y) is congruent mod 4 to one of the
 *          following: (1,1), (2,3), (3,2), (4,0); then R(m,n) is the number
 *          of UNmarked squares in the rectangle [0,m]x[0,n].
 * https://oeis.org/A143999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = R(m,n) where R(m,n)=m*n - floor(3*m*n/4) and (m,n) is the
 *            pair indexed by the nth entry read along antidiagonals of the
 *            rectangular array
 * Range:     u8 n <= 610, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned 128‑bit integer, returning
   the largest integer s.t. s*s <= x. */
static idx_t isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1 << 64); /* 2^64 */
    while (lo < hi)
    {
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
    if (n == 0)   /* undefined by OEIS, but return 0 safely */
        return 0;

    /* --- Find the diagonal d such that (d-1)*d/2 < n <= d*(d+1)/2 --- */
    unsigned __int128 t = (unsigned __int128)8 * n + 1;          /* 1 + 8*n */
    idx_t s = isqrt_u128(t);                                     /* floor(sqrt(t)) */
    idx_t d = (s - 1) / 2;                                       /* floor */
    /* adjust if necessary */
    while (((unsigned __int128)d * (d + 1)) / 2 < n)
        ++d;

    /* --- Position j within this diagonal (1‑based) --- */
    idx_t prev = ((unsigned __int128)(d - 1) * d) / 2;          /* terms before this diagonal */
    idx_t j = n - prev;                                         /* 1 <= j <= d */

    /* --- Convert (m,n) coordinates --- */
    idx_t m = j;                                                /* row index (1‑based) */
    idx_t k = d - j + 1;                                        /* column index (1‑based) */

    /* --- Compute R(m,k) = m*k - floor(3*m*k/4) --- */
    seq_t mn = (seq_t)m * (seq_t)k;
    seq_t R = mn - ((mn * 3) / 4);   /* integer division truncates correctly */

    return R;
}
