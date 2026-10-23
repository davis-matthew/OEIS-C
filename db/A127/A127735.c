/*
 * A127735  Triangle read by rows: A127701 * A002260 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A127735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value from the triangular matrix defined by T(n,n)=n^2
 *            and T(n,k)=k*(n+1) for 1 ≤ k < n, read by rows. For the 1‑based
 *            index n determine the row r by r(r+1)/2 ≥ n, let k = n –
 *            (r−1)r/2. Then a(n) = k*(r+1) if k<r, otherwise a(n)=k*k
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* find row r: minimal r with r*(r+1)/2 >= n */
    idx_t sq = _floor_isqrt_u64(1 + 8 * n);
    idx_t r = (sq - 1) / 2;
    idx_t tri = r * (r + 1) / 2;
    if (tri < n) {
        r++;
        tri = r * (r + 1) / 2;
    }

    /* column k in that row */
    idx_t prevTri = (r - 1) * r / 2;
    idx_t k = n - prevTri;

    /* compute value */
    if (k == r) {
        seq_t kk = (seq_t)k;
        return kk * kk;
    } else {
        seq_t kk = (seq_t)k;
        seq_t rr = (seq_t)r;
        return kk * (rr + 1);
    }
}
