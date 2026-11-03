/*
 * A110555  Triangle of partial sums of alternating binomial coefficients:
 *          T(n, k) = Sum_{j=0..k} binomial(n, j)*(-1)^j, for n >= 0, 0 <= k
 *          <= n.
 * https://oeis.org/A110555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = T(row_of_index(n), n −
 *            row_of_index(n)*(row_of_index(n)+1)/2). T(r,k) =
 *            binomial(r‑1,k)*(−1)^k for 0 ≤ k < r; T(r,r) = 0^r (with 0^0 =
 *            1)
 * Range:     i8 n <= 69, i16 n <= 197, i32 n <= 645, i64 n <= 1274,
 *            i128 n <= 1274, big any n
 * Validated: standard, pass, 90 terms through n = 1235 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static inline idx_t row_of_index(idx_t n)
{
    /* Binary search for the largest r with r*(r+1)/2 <= n. */
    idx_t low = 0, high = n, mid, tri;
    idx_t r = 0;
    while (low <= high)
    {
        mid = low + (high - low) / 2;
        tri = mid * (mid + 1) / 2;
        if (tri <= n) { r = mid; low = mid + 1; }
        else           high = mid - 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t r = row_of_index(n);                /* row number */
    if (r == 0) return 1;                    /* 0^0 = 1 case */

    idx_t k = n - r * (r + 1) / 2;            /* column within the row */
    if (k == r) return 0;                    /* (1−1)^r = 0 for r>0 */

    seq_t comb = binomial(r - 1, k);         /* binomial(r‑1, k) */
    if ((k & 1) == 0)
        return comb;
    else
        return -comb;
}
