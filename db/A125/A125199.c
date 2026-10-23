/*
 * A125199  Triangle read by rows: T(n,k) = 4*n*k - n - k, 1<=k<=n.
 * https://oeis.org/A125199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where * r = ceil( (sqrt(8*n + 1) - 1) / 2 ) (row
 *            index) * k = n - r*(r-1)/2 (column index) * T(r,k) = 4*r*k - r -
 *            k (triangle entry) * For n ≥ 1 this yields the OEIS a(n)
 * Range:     u8 n <= 43, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root of a non‑negative __int128 value.
 * Returns the largest idx_t s such that s*s ≤ x.  */
static inline idx_t floor_sqrt_u128(__int128 x)
{
    idx_t lo = 0, hi = (idx_t)1ULL << 32;          /* sqrt(max 8*n+1) < 2^34 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((__int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Return the n‑th term of OEIS A125199 (1‑based). */
seq_t a(idx_t n)
{
    /* Find row r; solve r(r+1)/2 ≥ n. */
    __int128 val = (__int128)8 * n + 1;          /* 8*n + 1  */
    idx_t s   = floor_sqrt_u128(val);           /* floor(sqrt(8*n+1)) */
    idx_t r   = (s - 1) >> 1;                    /* (s-1)/2  */
    /* Adjust r if necessary. */
    if ((__int128)r * (r + 1) / 2 < n)
        ++r;
    /* Number of terms before this row */
    idx_t prev = r - 1;
    idx_t prevCnt = (__int128)prev * (prev + 1) / 2;
    /* Column index (1‑based) */
    idx_t k = n - prevCnt;

    /* Compute T(r,k) = 4*r*k - r - k using a wider type for accuracy. */
    __int128 t = (__int128)4 * r * k - r - k;
    return (seq_t)t;   /* cast to seq_t (will be truncated if it does not fit) */
}
