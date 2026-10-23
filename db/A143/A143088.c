/*
 * A143088  Triangle T(n,m) = (2^(m+1) - 1) * (2^(n-m+1) - 1), read by rows, 0
 *          <= m <= n.
 * https://oeis.org/A143088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2^(m+1)-1)*(2^(n-m+1)-1) read by rows, * where m = n -
 *            r*(r+1)/2 and r = floor((sqrt(8*n+1)-1)/2)
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

/* Integer square root for 64‑bit unsigned integers
 * (fast algorithm based on binary search with shifts). */
static idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t shift = 1ULL << 30;          /* largest power of 4 <= 2^64-1 */
    while (shift > x)
        shift >>= 2;
    while (shift)
    {
        if (x >= r + shift)
        {
            x -= r + shift;
            r = (r >> 1) + shift;
        }
        else
        {
            r >>= 1;
        }
        shift >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* 1. Find row index r such that (r+1)(r+2)/2 > n */
    idx_t t = isqrt_u64(8 * n + 1) - 1;
    idx_t r = t >> 1;                 /* floor((sqrt(8*n+1)-1)/2) */

    /* 2. Compute column index m within that row */
    idx_t offset_before = r * (r + 1) / 2;
    idx_t m = n - offset_before;      /* 0 <= m <= r */

    /* 3. Evaluate the triangle entry T(r,m) = (2^(m+1)-1)*(2^(r-m+1)-1) */
    seq_t v1 = pow2(m + 1);
    seq_t v2 = pow2(r - m + 1);

    return (v1 - 1) * (v2 - 1);
}
