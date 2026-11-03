/*
 * A057036  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...;
 *          each k is an R(i(k),j(k)) and A057036(n)=i(2n-1).
 * https://oeis.org/A057036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = i(2n-1), where the integer k is placed in the rectangle
 *            R(i,j) with R(i,j) = C(i+j-1,2)+i. This implementation computes
 *            the i‑coordinate of the integer in the odd position (2n–1)
 * Range:     u8 n <= 68, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 81 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline seq_t odd_position_i(idx_t n)
{
    /* Position of the odd index */
    seq_t k = (seq_t)2 * (seq_t)n - 1;

    /* Find the smallest d such that d*(d+1)/2 >= k. */
    idx_t lo = 1, hi = 1;
    while ((seq_t)hi * (hi + 1) / 2 < k) {
        if (hi > (SIZE_MAX >> 1)) break;  /* safety for huge n */
        hi <<= 1;
    }
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t tri = (seq_t)mid * (mid + 1) / 2;
        if (tri >= k)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t d = lo;

    /* Compute i = k - C(d,2) = k - d*(d-1)/2 */
    seq_t start = (seq_t)d * (d - 1) / 2;
    seq_t i = k - start;
    return i;
}

seq_t a(idx_t n)
{
    return odd_position_i(n);
}
