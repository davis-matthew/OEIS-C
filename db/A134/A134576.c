/*
 * A134576  Triangle read by rows: T(n,k) = n^2*A051731(n,k).
 * https://oeis.org/A134576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r^2 if r % k == 0 (i.e., k divides r), else 0. * Here r
 *            and k are obtained by locating the n‑th entry in the * triangle
 *            T(r,k) = r^2 * [k | r]. The index n satisfies * r(r-1)/2 < n ≤
 *            r(r+1)/2 * and k = n - r(r-1)/2. These formulas follow directly
 *            from * the definition of A134576
 * Range:     u8 n <= 7, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Validated: standard, pass, 94 terms through n = 1201 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* n is 1‑based; for n == 0 (outside the offset) return 0. */
    if (n == 0)
        return 0;

    /* Find the row r such that r(r-1)/2 < n ≤ r(r+1)/2.  */
    seq_t val = (seq_t)8 * (seq_t)n + 1;        /* 8n+1 */
    seq_t s   = floor_sqrt(val);               /* floor(sqrt(8n+1)) */
    idx_t r   = (idx_t)((s - 1) / 2);           /* approximate r */

    /* Adjust r until the condition holds.  */
    while (((seq_t)r * ((seq_t)r + 1)) / 2 < (seq_t)n)
        r++;
    while (r > 0 &&
           (((seq_t)(r - 1) * (seq_t)r) / 2) >= (seq_t)n)
        r--;

    /* Column index within the row.  */
    idx_t k = n - (idx_t)(((seq_t)(r - 1) * (seq_t)r) / 2);

    /* Return r^2 if k divides r, otherwise 0.  */
    if (r % k == 0)
        return (seq_t)r * (seq_t)r;
    else
        return 0;
}
