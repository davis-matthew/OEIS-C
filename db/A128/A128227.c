/*
 * A128227  Right border (1,1,1,...) added to A002260.
 * https://oeis.org/A128227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle T(n,k) with T(n,k)=k for 1≤k<n and T(n,n)=1, flattened
 *            row by row
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 128‑bit numbers, result fits in uint64_t. */
static inline idx_t _isqrt128(__int128 x)
{
    idx_t lo = 0, hi = ((idx_t)1 << 35);   /* 2^35 > sqrt(8*2^64) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((__int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Find the row (r) containing the index n.  Rows are 1‑based and have
       r elements: 1,2,…,r‑1,1. */
    __int128 x = (__int128)n * 8 + 1;          /* 8*n+1  (may overflow uint64_t) */
    idx_t  sq  = _isqrt128(x);                /* floor sqrt(8*n+1) */
    idx_t  r   = (sq - 1) / 2;                /* initial guess for the row */

    /* Ensure r(r+1)/2 > n. */
    if ((__int128)r * (r + 1) / 2 <= n)
        ++r;

    /* Start index of row r (0‑based). */
    idx_t start = (__int128)(r - 1) * r / 2;
    idx_t offset = n - start;                 /* position within the row (0‑based) */

    if (offset == r - 1)        /* last element of the row */
        return (seq_t)1;
    else
        return (seq_t)(offset + 1);
}
