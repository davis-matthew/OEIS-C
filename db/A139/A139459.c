/*
 * A139459  Triangle read by rows: binomial(3*n,3*k), 0 <= k <= n.
 * https://oeis.org/A139459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = binomial(3*r, 3*k), where r = floor((sqrt(1 + 8*n) - 1) /
 *            2), k = n - r*(r+1)/2 (0 ≤ k ≤ r). The triangle is read row‑wise
 *            with rows of length r+1. The implementation uses only integer
 *            arithmetic, avoiding any floating‑point error for all n that fit
 *            in idx_t
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                // for the builtin uint64_t type
#include "../utils/binom.h"        // binom(idx_t a, idx_t b) → seq_t

/* Integer square root: floor(sqrt(x)) for x up to 2^64‑1. */
static idx_t floor_isqrt(idx_t x)
{
    if (x == 0 || x == 1)
        return x;

    idx_t low  = 0;
    idx_t high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        /* Prevent overflow: compare mid with x/mid instead of mid*mid. */
        if (mid > 0 && mid > x / mid) {
            high = mid - 1;
        } else {
            /* mid*mid <= x */
            if (mid * mid == x) {
                return mid;
            }
            low = mid + 1;
        }
    }
    return high;          /* high < low and high^2 < x < (high+1)^2 */
}

seq_t a(idx_t n)
{
    /* Compute the row index r as floor((sqrt(1+8*n)-1)/2) */
    idx_t D = n * 8 + 1;                    /* 1 + 8*n  (no overflow for n ≤ 2^61) */
    idx_t s = floor_isqrt(D);                /* floor(sqrt(D)) */
    idx_t r = (s - 1) / 2;                   /* floor((s-1)/2)   */

    /* Offset where row r starts in the flattened triangle */
    idx_t offset = r * (r + 1) / 2;

    /* Column index within the row */
    idx_t k = n - offset;                    /* 0 ≤ k ≤ r   */

    /* Compute the binomial coefficient binomial(3*r, 3*k) */
    return binom(3 * r, 3 * k);
}
