/*
 * A210196  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210195; see the Formula section.
 * https://oeis.org/A210196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the smallest integer with r(r+1)/2 ≥ n,
 *            k = n – r(r–1)/2 – 1, and T(r,0) = 1, T(r,k) = 2^(k+1) *
 *            binomial(r–1,k) for k > 0
 * Range:     u8 n <= 24, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

static inline idx_t _row_of(idx_t n)
{
    idx_t lo = 0, hi = 1;
    while ((unsigned __int128)hi * (hi + 1) / 2 < n) hi <<= 1;
    while (lo + 1 < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid;
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* unused, defensive guard */
    idx_t r = _row_of(n);
    idx_t prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - prev - 1;                    /* zero‑based column */
    if (k == 0) return (seq_t)1;
    seq_t bin = binomial(r - 1, k);
    seq_t p2  = pow2(k + 1);
    return bin * p2;                           /* multiplication works for all seq_t types */
}
