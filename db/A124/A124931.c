/*
 * A124931  Triangle read by rows: T(n,k) = (2*k-1)*binomial(n,k) (1 <= k <=
 *          n).
 * https://oeis.org/A124931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2*k-1)*binomial(r,k) where r = ceil((sqrt(8*n+1)-1)/2)
 *            is the row number, and k = n - r*(r-1)/2 is the position in that
 *            row
 * Range:     u8 n <= 7, u16 n <= 111, u32 n <= 449, u64 n <= 1918,
 *            u128 n <= 5050, big any n
 * Validated: standard, pass, 82 terms through n = 4805 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
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
    if (n == 0) return 0; /* safety for out‑of‑range input */

    /* find r: ceil((sqrt(8*n+1)-1)/2) */
    seq_t val = 8 * (seq_t)n + 1;
    seq_t d   = floor_sqrt(val);     /* floor(sqrt(val)) */
    idx_t r;
    if (d * d == val)                /* perfect square */
        r = ((idx_t)d - 1) / 2;
    else
        r = ((idx_t)d + 1) / 2;

    /* position k inside the row (1‑based) */
    idx_t prev = (r - 1) * r / 2;    /* sum of entries in rows < r */
    idx_t k    = n - prev;           /* 1 ≤ k ≤ r */

    /* compute T(r,k) = (2*k-1)*C(r,k) */
    seq_t bin = binomial(r, k);
    seq_t mult = (2 * (seq_t)k) - 1;
    return mult * bin;
}
