/*
 * A123963  Triangle T(n, k) = k^4 - n^4 + 2*k*n*(1 - k^2*n^2), read by rows.
 * https://oeis.org/A123963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, T(r,k) = k^4 - r^4 + 2*k*r*(1 - k^2*r^2), read by
 *            rows of the triangle
 * Range:     i8 n <= 7, i16 n <= 25, i32 n <= 592, i64 n <= 5150,
 *            i128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned 64‑bit values, using binary search
   with 128‑bit multiplication to avoid overflow. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x, best = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x)
            return mid;
        if (sq < (unsigned __int128)x) {
            best = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    /* Map the flat index n to row r and column k */
    idx_t r = (isqrt_u64(8ULL * n + 1) - 1) / 2;   /* row number */
    idx_t start = r * (r + 1) / 2;                 /* first index of this row */
    idx_t k = n - start;                           /* column in the row */

    /* Convert indices to seq_t for arithmetic */
    seq_t rr = (seq_t)r;
    seq_t kk = (seq_t)k;

    /* Compute k^4, r^4 and the product 2*k*r*(1 - k^2*r^2)
       using 128‑bit intermediates to avoid overflow when seq_t
       has small width. */
    __int128 k2 = (long long)kk * (long long)kk;
    __int128 r2 = (long long)rr * (long long)rr;
    __int128 k4 = k2 * k2;
    __int128 r4 = r2 * r2;
    __int128 temp = 1 - k2 * r2;                    /* 1 - k^2 * r^2 */
    __int128 prod = 2 * (long long)kk * (long long)rr * temp;

    __int128 result128 = k4 - r4 + prod;
    return (seq_t)result128;
}
