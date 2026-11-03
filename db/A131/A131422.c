/*
 * A131422  (A000012 * A127773) + (A127773 * A000012) - A000012.
 * https://oeis.org/A131422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r^2 + r + k^2 + k - 2)/2 where r = smallest integer with
 *            r(r+1)/2 >= n and k = n - r(r-1)/2
 * Range:     u8 n <= 7, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* the behavior for n=0 is undefined, return 0 */

    /* convert index to seq_t for arithmetic */
    seq_t nn = (seq_t)n;

    /* find r by solving r(r+1)/2 >= n */
    seq_t val = (seq_t)8 * nn + 1;          /* 8n+1 */
    seq_t s   = floor_sqrt(val);            /* floor(sqrt(8n+1)) */
    idx_t r   = (idx_t)((s - 1) / 2);        /* initial guess floor((sqrt-1)/2) */

    /* correct r if needed */
    if ((seq_t)r * (seq_t)(r + 1) / 2 < nn)
        r++;

    /* number of elements before row r */
    seq_t prev = (seq_t)(r - 1) * (seq_t)r / 2;

    /* column index k (1‑based) */
    idx_t k = (idx_t)(nn - prev);

    /* compute T(r,k) = (r^2 + r + k^2 + k - 2)/2 */
    seq_t rr   = (seq_t)r;
    seq_t kk   = (seq_t)k;
    seq_t num = rr * rr + rr + kk * kk + kk - 2;
    seq_t ans = num / 2;
    return ans;
}
