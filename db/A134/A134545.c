/*
 * A134545  Triangle read by rows: T(n,k) = Sum_{i=k..n} (i+1-k)*A051731(n,i).
 * https://oeis.org/A134545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{d|r, d≥k} (d + 1 – k) where (r, k) is the row and
 *            column of n in the triangle read row by row. r is the smallest
 *            integer with r(r+1)/2 ≥ n, and k = n – r(r–1)/2
 * Range:     u8 n <= 7, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Validated: standard, pass, 107 terms through n = 1180 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the supplied int‑sized types     */
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
    if (n == 0)          /* sequence is defined from index 1 */
        return 0;

    /* 1. Find the row index r.  */
    seq_t temp = (seq_t)n * 8 + 1;          /* 8*n + 1  */
    seq_t s = floor_sqrt(temp);            /* floor(sqrt(8*n+1)) */
    idx_t r = (idx_t)((s - 1) / 2);         /* floor((sqrt(8n+1)-1)/2) */

    /* check whether we need to bump r one more */
    seq_t tri = (seq_t)r * (r + 1) / 2;
    if (tri < (seq_t)n)
        ++r;

    /* 2. Compute column index k.  */
    seq_t triPrev = (seq_t)(r - 1) * r / 2; /* triangular number of r-1 */
    idx_t k = (idx_t)(n - triPrev);        /* 1 ≤ k ≤ r */

    /* 3. Sum over divisors d of r with d≥k.  */
    seq_t result = 0;
    for (idx_t d = 1; d <= r / d; ++d) {   /* d*d <= r without overflow */
        if (r % d == 0) {
            idx_t e = r / d;

            if (d >= k)
                result += (seq_t)d - (seq_t)k + 1;

            if (e != d && e >= k)
                result += (seq_t)e - (seq_t)k + 1;
        }
    }
    return result;
}
