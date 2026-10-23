/*
 * A145890  Triangle read by rows: T(n,k) = B(k)C(n-k), where B(j) is the
 *          central binomial coefficient binomial(2j,j) (A000984) and C(j) is
 *          the Catalan number binomial(2j,j)/(j+1) (A000108); 0 <= k <= n.
 * https://oeis.org/A145890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2) and k = n -
 *            r*(r+1)/2, and T(n,k) = binomial(2k,k) * binomial(2n-2k,n-k) /
 *            (n-k+1). The sequence lists the triangle T(n,k) row‑wise (by
 *            increasing n, and for each n, by increasing k)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* Utility function provided by the build system; can be omitted with a
   local implementation if desired. */
#include "../utils/binom.h"

static inline idx_t
isqrt_u64(idx_t n)          /* integer sqrt, floor of sqrt(n) */
{
    idx_t lo = 0;
    idx_t hi = 1;
    /* grow hi until hi*hi > n (using 128‑bit to avoid overflow) */
    while ((unsigned __int128)hi * hi <= n) hi <<= 1;
    /* binary search on [lo, hi) */
    while (lo + 1 < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ((unsigned __int128)mid * mid <= n) lo = mid;
        else hi = mid;
    }
    return lo;
}

seq_t
a(idx_t n)
{
    /* Find the row r containing the n‑th term (row‑major order). */
    idx_t m   = 8 * n + 1;          /* 8*n+1 fits in idx_t for given limits */
    idx_t sq  = isqrt_u64(m);       /* floor(sqrt(8*n+1)) */
    idx_t r   = (sq - 1) / 2;        /* floor((sqrt(8*n+1)-1)/2) */
    idx_t row_start = r * (r + 1) / 2;
    idx_t k   = n - row_start;      /* column within the row */

    /* Compute T(r,k) = binom(2k,k) * binom(2(r-k),r-k) / (r-k+1) */
    seq_t b1   = binom(2 * k, k);
    seq_t b2   = binom(2 * (r - k), r - k);
    seq_t prod = b1 * b2;
    seq_t denom = (seq_t)(r - k + 1);
    return prod / denom;
}
