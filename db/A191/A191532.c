/*
 * A191532  Triangle T(n,k) read by rows: T(n,n) = 2n+1, T(n,k)=k for k<n.
 * https://oeis.org/A191532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = * Let r = floor((sqrt(8n+1)-1)/2). Let k = n - r(r+1)/2.
 *            * If k == r then a(n) = 2r+1 else a(n) = k. * This is the
 *            flattening of the triangle T(n,k) with * T(n,k) = k for k < n *
 *            T(n,n) = 2n+1
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/*  Compute the starting index of row r:
 *  start(r) = r*(r+1)/2
 */
static inline idx_t row_start(idx_t r)
{
    /* Use 128‑bit arithmetic to avoid overflow for large r */
    return (idx_t)((( __int128)r * (r + 1)) / 2);
}

/*  Find the row r that contains the overall index n.
 *  This is the unique r with:
 *      r*(r+1)/2 ≤ n < (r+1)*(r+2)/2
 */
static inline idx_t find_row(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = 1;

    /* Grow hi so that row_start(hi) > n */
    while (row_start(hi) <= n)
        hi <<= 1;

    /* Binary search for largest r with row_start(r) <= n */
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo + 1) / 2;    /* upper mid to avoid infinite loop */
        if (row_start(mid) <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;   /* lo == hi */
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    idx_t k = n - row_start(r);   /* offset in the row, 0 ≤ k ≤ r */

    if (k == r)
    {
        /* Return 2*r + 1, computed in a wide type then cast to seq_t */
        __int128 val = ( __int128)r * 2 + 1;
        return (seq_t)val;
    }
    else
    {
        return (seq_t)k;
    }
}
