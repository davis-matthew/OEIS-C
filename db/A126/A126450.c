/*
 * A126450  Triangle, read by rows, where T(n,k) = C( C(n+2,3) - C(k+2,3) + 1,
 *          n-k) for n>=k>=0.
 * https://oeis.org/A126450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) with * row = floor((sqrt(8n+1)-1)/2) * col =
 *            n - row*(row+1)/2 * T(r,k) = C( C(r+2,3) - C(k+2,3) + 1 , r-k )
 *            * (OEIS A126450)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 27, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Integer square root: floor(sqrt(x)) for 0 <= x < 2^64 */
static idx_t floor_sqrt(idx_t x)
{
    idx_t lo = 0, hi = 1ULL << 32;          /* sqrt(2^64-1) < 2^32 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > x / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Compute binomial coefficient C(n,3) using 64‑bit arithmetic safely */
static idx_t choose3(idx_t n)
{
    if (n < 3) return 0;
    __int128 v = (__int128)n * (n - 1) * (n - 2) / 6;
    return (idx_t)v;
}

seq_t a(idx_t n)
{
    /* Locate the (row, col) position within the triangular array. */
    idx_t row = (floor_sqrt(8 * n + 1) - 1) / 2;
    idx_t prev_len = row * (row + 1) / 2;      /* terms before this row */
    idx_t col = n - prev_len;

    /* Compute the internal binomial parameters. */
    idx_t cn3   = choose3(row + 2);            /* C(row+2,3) */
    idx_t ck3   = choose3(col + 2);            /* C(col+2,3) */
    idx_t base  = cn3 - ck3 + 1;               /* C(row+2,3) - C(col+2,3) + 1 */
    idx_t m     = row - col;                   /* r-k */

    /* Final binomial: C(base, m) */
    return binomial(base, m);
}
