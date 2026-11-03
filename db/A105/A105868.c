/*
 * A105868  Triangle read by rows, T(n,k) = C(n,k)*C(k,n-k).
 * https://oeis.org/A105868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(r,k) * binomial(k,r-k) where r = row index of n
 *            (such that r(r+1)/2 ≤ n < (r+1)(r+2)/2), k = n - r(r+1)/2
 * Range:     u8 n <= 40, u16 n <= 98, u32 n <= 314, u64 n <= 1062,
 *            u128 n <= 1274, big any n
 * Validated: standard, pass, 96 terms through n = 1083 (2026-11-01)
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
    idx_t r = row_of_index(n);
    idx_t k = n - r * (r + 1) / 2;
    seq_t left  = binomial(r, k);
    seq_t right = binomial(k, r - k);
    return left * right;
}
