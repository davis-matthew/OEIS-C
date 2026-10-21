/*
 * A088231  Triangle read by rows: T(n,k) = number of squares not less than
 *          n-k and not greater n+k, 0<=k<n.
 * https://oeis.org/A088231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of integers s for which (n‑k) ≤ s² ≤ (n+k), where
 *            k is the offset of n in the triangular indexing of the triangle
 *            T(n,k) with 0 ≤ k < n
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The triangle is read by rows: first T(1,0), then T(2,0),T(2,1), then
   T(3,0),T(3,1),T(3,2), … */
#include <stdint.h>

static inline seq_t floor_sqrt(seq_t x)
{
    seq_t r = 0;
    while ((r + 1) * (r + 1) <= x)
        ++r;
    return r;
}

seq_t a(idx_t N)
{
    /* Convert the linear index N (≥1) to triangle row r and column k
       such that N = 1 + (r-1)r/2 + k, with 0 ≤ k < r. */
    idx_t low = 1, high = N;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (mid * (mid + 1) / 2 >= N)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                        /* row number, 1‑based */
    idx_t start = (r - 1) * r / 2 + 1;    /* first index of this row */
    idx_t k = N - start;                  /* column offset, 0‑based */

    /* Count squares s² with (r–k) ≤ s² ≤ (r+k) */
    seq_t low_sq  = (seq_t)r - (seq_t)k;
    seq_t high_sq = (seq_t)r + (seq_t)k;

    seq_t s_low  = floor_sqrt(low_sq);
    if (s_low * s_low < low_sq)
        ++s_low;

    seq_t s_high = floor_sqrt(high_sq);

    if (s_low > s_high)
        return 0;
    return s_high - s_low + 1;
}
