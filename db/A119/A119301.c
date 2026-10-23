/*
 * A119301  Triangle read by rows: T(n,k) = binomial(3*n-k,n-k).
 * https://oeis.org/A119301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(3*r - c, r - c) where r = floor((sqrt(8*n+1) -
 *            1)/2) (the row index), c = n - r*(r+1)/2 (the column offset).
 *            The returned value is the flattening of the triangle T(n,k) =
 *            binomial(3*n-k, n-k) by rows
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* provides binomial(idx_t,idx_t) */

seq_t a(idx_t n)
{
    /* Find largest r with r*(r+1)/2 <= n.  */
    idx_t low  = 0;
    idx_t high = 1;

    /* Grow high until the triangular number exceeds n.
       Use __int128 to avoid overflow. */
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
    {
        high <<= 1;               /* high *= 2 */
        if (high == 0)            /* guard against wrap‑around */
            break;
    }

    /* Binary search between low and high. */
    while (low < high)
    {
        idx_t mid = low + (high - low + 1) / 2;   /* round up */
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;

        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }

    idx_t r   = low;
    idx_t col = n - r * (r + 1) / 2;       /* c */

    idx_t numer = 3 * r - col;             /* 3*n - k */
    idx_t denom = r - col;                 /* n - k   */

    return binomial(numer, denom);
}
