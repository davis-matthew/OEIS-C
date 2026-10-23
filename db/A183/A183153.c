/*
 * A183153  T(n,k) is the number of order-preserving partial isometries of an
 *          n-chain of height k (height of alpha = |Im(alpha)|).
 * https://oeis.org/A183153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,c) where r = ⌊(√(8 n+1)−1)/2⌋, c = n−r(r+1)/2,
 *            T(r,0)=1, T(r,k)=(2r−k+1)·C(r,k)/(k+1)
 * Range:     u8 n <= 29, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------  Dependencies ------------- */
#include <stdint.h>
#include "../utils/binom.h"

/* -------------  Integer square‑root ------------- */
static inline idx_t isqrt(idx_t v)
{
    idx_t lo = 0, hi = v;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (mid <= v / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* -------------  Main function ------------- */
seq_t a(idx_t n)
{
    /* find the row r that contains index n */
    idx_t r = (isqrt(8 * n + 1) - 1) / 2;

    /* index of the first element of this row (0‑based) */
    idx_t prev = r * (r + 1) / 2;
    idx_t c = n - prev;   /* 0 ≤ c ≤ r */

    if (c == 0)
        return (seq_t)1;          /* T(r,0) = 1 */

    /* compute T(r,c) = (2r−c+1)·C(r,c)/(c+1) */
    seq_t bin = binom(r, c);                 /* C(r,c)   */
    bin *= (seq_t)(2 * r - c + 1);           /* numerator */
    bin /= (seq_t)(c + 1);                   /* denominator */
    return bin;
}
