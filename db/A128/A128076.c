/*
 * A128076  Triangle T(n,k) = 2*n-k, read by rows.
 * https://oeis.org/A128076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2*(t+1) + (t*(t+1)/2 - n) where t =
 *            floor((sqrt(8*n-7)-1)/2). This yields the sequence 1, 3, 2, 5,
 *            4, 3, 7, 6, 5, 4, 9, 8, … (OEIS A128076)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned 64‑bit values.
   Returns floor(sqrt(x)). */
static inline idx_t isqrt_u64(idx_t x)
{
    if (x < 2) return x;
    idx_t lo = 0, hi = ((idx_t)1 << 32);   /* sqrt(2^64-1) < 2^32 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > x / mid) hi = mid - 1;
        else lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    idx_t m = 8 * n - 7;            /* 8*n-7  (fits for n up to ~2^63-1) */
    idx_t k = isqrt_u64(m);         /* floor(sqrt(8*n-7)) */
    idx_t t = (k - 1) / 2;          /* floor((sqrt(8*n-7)-1)/2) */

    seq_t tt   = (seq_t)t;
    seq_t start = 2 * (tt + 1);      /* 2*(t+1) */
    seq_t tri   = tt * (tt + 1) / 2; /* t*(t+1)/2 */
    seq_t res   = start + (tri - (seq_t)n);

    return res;
}
