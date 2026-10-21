/*
 * A211649  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^2+x^2+y^2=2n.
 * https://oeis.org/A211649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (w,x,y) with 1 ≤ w,x,y ≤ n such
 *            that w² + x² + y² = 2·n
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Integer square‑root for a 64‑bit unsigned integer.
   Returns ⌊√v⌋.  Uses binary search with 128‑bit to avoid overflow. */
static inline idx_t isqrt_u64(uint64_t v)
{
    idx_t low = 0, high = v;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == (unsigned __int128)v) {
            return mid;
        } else if (sq < (unsigned __int128)v) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;          /* floor sqrt */
}

seq_t a(idx_t n)
{
    /* No solutions when n == 0 (2·n-1 is negative). */
    if (n == 0) return (seq_t)0;

    /* 2·n fits in seq_t because n is at most a few thousand. */
    seq_t two_n = (seq_t)2 * (seq_t)n;

    /* Maximum w is floor(√(2n‑1)). */
    idx_t w_max = isqrt_u64((uint64_t)2 * (uint64_t)n - 1);

    seq_t count = 0;

    for (idx_t w = 1; w <= w_max; ++w) {
        seq_t wsq = (seq_t)w * (seq_t)w;           /* w² */
        seq_t rem1 = two_n - wsq;                  /* 2n - w² */

        /* x must satisfy 1 ≤ x ≤ √(rem1‑1).  If rem1<=1, this loop skips. */
        idx_t x_max = 0;
        if (rem1 > (seq_t)1) {
            x_max = isqrt_u64((uint64_t)rem1 - 1);
        }

        for (idx_t x = 1; x <= x_max; ++x) {
            seq_t xsq = (seq_t)x * (seq_t)x;       /* x² */
            seq_t rem2 = rem1 - xsq;               /* remaining for y² */

            /* rem2 ≥ 1 by construction. */
            idx_t y = isqrt_u64((uint64_t)rem2);
            if ((seq_t)y * (seq_t)y == rem2 && y <= n) {
                ++count;                           /* (w,x,y) is a valid triple */
            }
        }
    }

    return count;
}
