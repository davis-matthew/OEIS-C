/*
 * A135065  A127733 * A007318 as infinite lower triangular matrices.
 * https://oeis.org/A135065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r,c) * (r+1)^2 * r = floor((sqrt(8*n+1)-1)/2) *
 *            c = n - r*(r+1)/2 * This yields the row‑major traversal of the
 *            triangle * T(n,k)=binomial(n,k)*(n+1)^2
 * Range:     u8 n <= 16, u16 n <= 70, u32 n <= 361, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Integer square root for 64‑bit values. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 32;       /* sqrt(2^64-1) < 2^32 */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t t = 8 * n + 1;
    idx_t s = isqrt_u64(t);
    idx_t r = (s - 1) / 2;              /* row index */
    idx_t start = r * (r + 1) / 2;      /* first index of this row */
    idx_t c = n - start;                /* column index */
    seq_t val = binomial(r, c);         /* binomial coefficient */
    seq_t mult = (seq_t)(r + 1) * (seq_t)(r + 1);
    return val * mult;
}
