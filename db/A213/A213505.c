/*
 * A213505  Rectangular array: (row n) = b**c, where b(h) = h^2, c(h) =
 *          (n-1+h)^2, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: standard, pass, 66 terms through n = 77 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = value obtained by reading the reciprocal
 * rectangular array T(r,c) = sum_{h=1}^{c} h^2 * (r+c-h)^2
 * along its falling antidiagonals.  For each n ≥ 1,
 * let S be the smallest integer with (S-1)*S/2 ≥ n.
 * Then
 *   row = n - (S-2)*(S-1)/2
 *   col = S - row
 * and a(n) = Σ_{h=1}^{col} h^2 * (row+col-h)^2.
 */

#include <stdint.h>

static idx_t _find_s(idx_t n)
{
    /* binary search for the smallest S with (S-1)*S/2 ≥ n */
    idx_t lo = 2, hi = 2;
    while (((__int128)hi * (hi - 1) / 2) < n)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (((__int128)mid * (mid - 1) / 2) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* not used; offset is 1 */

    idx_t S = _find_s(n);
    /* count of elements before this diagonal */
    idx_t before = (idx_t)((__int128)(S - 2) * (S - 1) / 2);
    idx_t offset = n - before;          /* 1 ≤ offset ≤ S-1 */
    idx_t row = offset;
    idx_t col = S - offset;

    seq_t sum = (seq_t)0;
    for (idx_t h = 1; h <= col; ++h) {
        seq_t h2 = (seq_t)h * h;            /* h^2 */
        idx_t m = row + col - h;
        seq_t m2 = (seq_t)m * m;            /* (row+col-h)^2 */
        sum += h2 * m2;
    }
    return sum;
}
