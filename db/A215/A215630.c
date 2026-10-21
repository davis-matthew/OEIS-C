/*
 * A215630  Triangle read by rows: T(n,k) = n^2 - n*k + k^2, 1 <= k <= n.
 * https://oeis.org/A215630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(r,k) = r^2 - r*k + k^2 (1 ≤ k ≤ r), flattened by rows. a(n) is
 *            the nth term when the triangle is read by rows, n ≥ 1
 * Range:     u8 n <= 135, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

/* ------------------------------------------------------------------
 * Integer square root for values up to 2^128‑1.
 * Returns floor(sqrt(v)).
 * ------------------------------------------------------------------ */
static idx_t _isqrt_u128(unsigned __int128 v)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64) - 1;
    unsigned __int128 mid, best = 0;

    while (low <= high) {
        mid = (low + high) / 2;
        if (mid * mid <= v) {
            best = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return (idx_t)best;
}

/* ------------------------------------------------------------------
 * Sequence a(n): returns the nth term of the OEIS triangle A215630.
 * ------------------------------------------------------------------ */
extern seq_t a(idx_t n)
{
    /* Find r such that r(r+1)/2 ≥ n and (r-1)r/2 < n.          */
    unsigned __int128 t = ((unsigned __int128)8 * n) + 1;      /* 8n+1 */
    idx_t sqrt_t = _isqrt_u128(t);                            /* floor sqrt(8n+1) */
    idx_t r = (sqrt_t - 1) >> 1;                               /* floor((sqrt-1)/2) */

    /* Adjust r to the smallest integer with r(r+1)/2 ≥ n.         */
    if (((unsigned __int128)r * (r + 1)) / 2 < n)
        ++r;

    /* Compute k = n - sum_{i=1}^{r-1} i = n - (r-1)r/2.          */
    idx_t k = n - (((unsigned __int128)(r - 1) * r) / 2);

    /* Compute T(r,k) = r^2 - r*k + k^2 in seq_t.                 */
    seq_t rr = (seq_t)r;
    seq_t kk = (seq_t)k;
    seq_t val = rr * rr - rr * kk + kk * kk;
    return val;
}
