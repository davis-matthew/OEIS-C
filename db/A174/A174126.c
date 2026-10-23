/*
 * A174126  Triangle T(n, k) = (n-k)^2 * binomial(n-1, k-1)^2 with T(n, 0) =
 *          T(n, n) = 1, read by rows.
 * https://oeis.org/A174126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = flattening of triangle T(n,k) = (n-k)^2 * binomial(n-1,
 *            k-1)^2 with T(n,0)=T(n,n)=1 (OEIS A174126)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 542, u128 n <= 2108,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Compute integer floor sqrt of x for unsigned 64‑bit x */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = (idx_t)1 << 32, mid, sqrt = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if ((seq_t)mid * (seq_t)mid <= (seq_t)x) {
            sqrt = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return sqrt;
}

/* Row r and column k for the global index n */
static void row_col(idx_t n, idx_t *r, idx_t *k)
{
    /* solve r(r+1)/2 <= n < (r+1)(r+2)/2   ;  r >= 0   */
    idx_t val = 8 * n + 1;
    idx_t sq = isqrt_u64(val);          /* floor sqrt(8*n+1) */
    *r = (sq - 1) / 2;                   /* floor part of formula */
    /* start index of the row */
    idx_t start = (*r) * (*r + 1) / 2;
    *k = n - start;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t r, k;
    row_col(n, &r, &k);

    if (k == 0 || k == r)     /* boundaries of the row */
        return (seq_t)1;

    /* compute (r-k)^2 * binomial(r-1,k-1)^2 */
    idx_t diff = r - k;
    seq_t diff_sq = (seq_t)diff * (seq_t)diff;
    seq_t w = binomial(r - 1, k - 1);
    seq_t res = diff_sq * w * w;      /* w*w may overflow for 64‑bit, but
                                        * that is acceptable for values
                                        * that fit in seq_t */
    return res;
}
