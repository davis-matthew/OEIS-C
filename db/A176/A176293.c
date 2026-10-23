/*
 * A176293  Triangle T(n,k) = 1 + 2*k*(n-k)*(n-1)^2, read by rows.
 * https://oeis.org/A176293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r = ⌊(√(8n+1)-1)/2⌋, k = n - r(r+1)/2 * and
 *            T(r,k) = 1 + 2*k*(r-k)*(r-1)^2 (triangle read by rows)
 * Range:     u8 n <= 22, u16 n <= 216, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit unsigned value, safe for all inputs.
 * Returns ⌊√v⌋.  Uses a binary search that never divides by zero. */
static idx_t isqrt_u64(idx_t v)
{
    if (v < 2u) return v;

    idx_t low   = 1u;
    idx_t high  = v >> 1;          /* v/2 is an upper bound for √v */
    idx_t ans   = 1u;

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t div = v / mid;

        if (mid > div)          /* mid*mid > v */
            high = mid - 1u;
        else {                  /* mid*mid <= v */
            ans  = mid;
            low  = mid + 1u;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.            */
    idx_t m = 8u * n + 1u;                     /* 8n+1 fits in idx_t       */
    idx_t sqrt_m = isqrt_u64(m);               /* floor(sqrt(8n+1))        */
    idx_t r = (sqrt_m - 1u) / 2u;              /* floor((sqrt(8n+1)-1)/2)  */

    /* Column k within the row.                                            */
    idx_t prev_total = r * (r + 1u) / 2u;        /* terms before row r  */
    idx_t k = n - prev_total;                   /* position in the row */

    /* Compute T(r,k) = 1 + 2*k*(r-k)*(r-1)^2.  Use seq_t at every step.  */
    seq_t nn = (seq_t)r;
    seq_t kk = (seq_t)k;

    seq_t term = (seq_t)1 + (seq_t)2 * kk * (nn - kk) *
                        ((nn - (seq_t)1) * (nn - (seq_t)1));
    return term;
}
