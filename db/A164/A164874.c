/*
 * A164874  Triangle read by rows: T(1,1)=2; T(n,k)=2*T(n-1,k)+1, 1<=k<n;
 *          T(n,n)=2*(T(n-1,n-1)+1).
 * https://oeis.org/A164874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(r+1) - 2^(r-c) - 1 * where r is the row such that
 *            r*(r+1)/2 ≥ n * and c = n - r*(r-1)/2 (column within that row) *
 *            The triangle T(n,k) satisfies T(n,k) = 2^(n+1) - 2^(n-k) - 1
 * Range:     u8 n <= 28, u16 n <= 120, u32 n <= 496, u64 n <= 2016,
 *            u128 n <= 5050, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 1‑based index; n >= 1 */
    if (n == 0) {  /* undefined, but return 0 for safety */
        return 0;
    }

    /* Find the smallest r with r*(r+1)/2 >= n */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= n) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    idx_t r = lo;                     /* row number */
    idx_t prev_tri = r * (r - 1) / 2;  /* triangles up to previous row */
    idx_t c = n - prev_tri;           /* column within row r, 1‑based */

    /* Compute the term using the closed‑form for T(r,c) */
    return pow2(r + 1) - pow2(r - c) - 1;
}
